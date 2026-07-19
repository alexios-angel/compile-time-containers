# ctc — compile-time containers

Header-only C++20 library of fixed-capacity `constexpr` containers — **every
container in the C++20 standard library**, plus string and pair — that stay
[**structural
types**](https://en.cppreference.com/w/cpp/language/template_parameters), so a
value you build at compile time can be a non-type template parameter, persist
to runtime in static storage, or both:

```cpp
#include <ctc.hpp>

constexpr auto greeting = [] {
    ctc::string<32> s;
    s.append("Hello, ");
    s.append("World");
    s.push_back('!');
    return s;
}();
static_assert(greeting == "Hello, World!");
static_assert(greeting.starts_with("Hello") && greeting.find("World") == 7);

template <auto V> struct takes_a_container { };       // containers as NTTPs
using t = takes_a_container<ctc::vector<int, 4>{1, 2, 3}>;
```

`ctc` is the shared containers layer of a family of compile-time libraries —
[compile-time-lark](https://github.com/alexios-angel/compile-time-lark),
[compile-time-json](https://github.com/alexios-angel/compile-time-json)
(and [json5](https://github.com/alexios-angel/compile-time-json5) /
[yaml](https://github.com/alexios-angel/compile-time-yaml) /
[xml](https://github.com/alexios-angel/compile-time-xml)) and
[notre](https://github.com/alexios-angel/notre) — which today each hand-roll
these data structures.

## The types

Everything lives in `include/ctc/`, umbrella header `<ctc.hpp>`, namespace
`ctc`. The capacity is a template parameter; the size is a runtime (or
constexpr) value. All operations are `constexpr`; the library is
exception-free and warning-clean under `-pedantic -Wall -Wextra -Werror
-Wconversion`.

Every C++20 standard container has a ctc counterpart:

| std (C++20) | ctc | Notes |
| --- | --- | --- |
| `array` | `ctc::array<T, N>` | Aggregate, brace elision, CTAD. |
| `vector` | `ctc::vector<T, N>` | An `inplace_vector` that stays structural. `T` must be default-constructible. |
| `deque` | `ctc::deque<T, N>` | The whole vector API plus `push_front`/`pop_front`. Not a ring: the layout stays canonical (front ops are O(size) — irrelevant at compile-time sizes). |
| `forward_list` | `ctc::forward_list<T, N>` | Index-linked nodes in a flat array; stable iterators, `insert_after`/`erase_after`. |
| `list` | `ctc::list<T, N>` | Doubly index-linked; `insert`/`erase` anywhere, bidirectional iterators. |
| `set` / `multiset` | `ctc::set` / `ctc::multiset<K, N, Compare>` | Sorted, binary-search lookups, transparent heterogeneous `Compare` (default `ctc::less`). |
| `map` / `multimap` | `ctc::map` / `ctc::multimap<K, V, N, Compare>` | Sorted; `m[key] = v`, `insert_or_assign`, `try_emplace`, `equal_range`. |
| `unordered_set` … `unordered_multimap` | `ctc::unordered_*<…, Hash, KeyEqual>` | Flat, linear `KeyEqual` scans — and iteration in **insertion order, guaranteed** (an `unordered_map` models a JSON object). `Hash` is accepted and ignored: at compile-time sizes a hash table buys nothing and would poison NTTP identity. |
| `stack` / `queue` / `priority_queue` | `ctc::stack` / `queue` / `priority_queue<T, N, …>` | The underlying container is a public member `c` (public keeps the adaptor structural). `priority_queue` is a real binary heap. |
| `span` | *use `std::span`* | Already fully constexpr in C++20. |
| *(strings library)* `basic_string` | `ctc::basic_string<CharT, N>` | Code-unit storage, always null-terminated (`c_str()`); aliases `string<N>`, `wstring<N>`, `u8string<N>`, `u16string<N>`, `u32string<N>`. |
| *(utility)* `pair` | `ctc::pair<F, S>` | A structural `std::pair` (aggregate, two public members) — `std::pair` is not guaranteed structural. |

API highlights:

- `basic_string`: `push_back`/`pop_back`/`append`/`+=`/`resize`/`clear`,
  `find`/`rfind`/`contains`/`starts_with`/`ends_with`/`compare`,
  `substr<Pos, Count>()` (a right-sized copy) and `substr(pos, count)` (a
  view), `operator+` concatenation, comparisons against anything convertible
  to `std::string_view`, implicit conversion to `std::string_view`, iostream
  `<<`. Construction from literals deduces the capacity:
  `ctc::basic_string{"hi"}` is a `ctc::string<2>` (or `ctc::make_string("hi")`;
  alias-template deduction needs clang ≥ 19). Unlike `ctll::fixed_string`
  (which decodes to UTF-32 code points), the content is stored as written.

- `vector`: `push_back`/`emplace_back`/`pop_back`/`insert`/`erase`/`resize`/
  `clear`, `operator[]`/`at`/`front`/`back`/`data`, iterators, element-wise
  `==` across capacities. Elements can be aggregates, `ctc::pair`s, strings,
  other vectors…

- sorted associative (`set`/`map`/`multi*`): lookups are **heterogeneous
  through the transparent comparator** — a `std::string_view` or a string
  literal finds a `ctc::string` key. Comparators must be stateless (they are
  constructed on use, never stored, so they never appear in the NTTP).
  Because a sorted-unique layout is canonical, two equal `set`s/`map`s are
  the *same template argument* regardless of insertion order.

- unordered associative: the same heterogeneous lookups via `KeyEqual`;
  iteration order is insertion order, guaranteed — `unordered_map` is the
  JSON-object container the sibling parsers need.

- `forward_list`/`list` are the one exception to layout canonicality: node
  placement depends on operation history, so equal contents guarantee only
  `==`, not NTTP identity — `shrunk<V>()`/`with_capacity()` rebuilds a list
  compactly and canonically when identity matters.

## Oversize, then right-size

Constexpr allocation cannot escape to runtime in C++20, so the capacity is a
template parameter. When the final size is only known after building, build in
a generous buffer and shrink the *value*:

```cpp
constexpr auto build() {              // returns ctc::vector<int, 1024>
    ctc::vector<int, 1024> v;
    for (int i = 2; i < 100; i += 3) v.push_back(i);
    return v;
}

constexpr auto big = build();         // 1024 slots
constexpr auto v = ctc::shrunk<big>();   // ctc::vector<int, 33> — exactly v.size()
```

`shrunk<V>()` works on every ctc container (it is
`V.with_capacity<V.size()>()` behind the scenes). Only the right-sized value
occupies static storage in your binary.

## Structural, all the way down

Non-type template parameters require structural types, so every container is a
plain public array plus a public size — no unions, nothing private. Two
guarantees make NTTP identity behave:

- every slot past `size()` is kept equal to `T{}` by the mutators
  (`pop_back`, `erase`, `clear`, `resize` reset vacated slots), so **equal
  contents mean equivalent template arguments** no matter how the value was
  built;

- `ctc::pair` exists because `std::pair` is not guaranteed structural.

The price: `vector<T, N>` requires a default-constructible `T` (the whole
array exists up front). That is the trade that keeps NTTP support — a
`union`-based lazy-construction vector cannot be structural.

## Preconditions

Overflowing the capacity, indexing out of range, `at()` with a missing key,
`pop_back()` on empty — all call a non-`constexpr` trap function. In constant
evaluation that is a **compile error** whose backtrace names the violation
(e.g. `ctc::vector: push_back on a full vector`); at runtime it aborts.
Nothing throws.

## Using it

Copy `include/` (it is self-contained, no dependencies), or:

```bash
make            # compile the static_assert test suites (compiling IS the test)
make CXX=clang++
make single-header   # generate single-header/ctc.hpp (needs python3 + quom)
```

```cmake
find_package(ctc REQUIRED)            # or add_subdirectory / FetchContent
target_link_libraries(app PRIVATE ctc::ctc)
```

CMake ≥ 3.14. `cmake -B build && cmake --build build && ctest --test-dir
build` builds the test suites and examples; `-DCTC_MODULE=ON` builds the
experimental C++23 module (`import ctc;`). Packaging (DEB/RPM/pkg-config) via
CPack, as in the sibling repos.

## Supported compilers

GCC ≥ 12 and Clang ≥ 15, `-std=c++20` (CI builds every suite with GCC 12–14
and Clang 15/16/18). MSVC is untested.

## License

[MIT](LICENSE).
