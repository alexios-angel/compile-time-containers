# ctc — compile-time containers

Header-only C++20 library of fixed-capacity `constexpr` containers — **string,
vector, map, pair** — that stay [**structural
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

| Type | What it is |
| --- | --- |
| `ctc::basic_string<CharT, N>` | Fixed-capacity string of `CharT` code units, always null-terminated (`c_str()`). Aliases `string<N>`, `wstring<N>`, `u8string<N>`, `u16string<N>`, `u32string<N>`. |
| `ctc::vector<T, N>` | Fixed-capacity vector — an `inplace_vector` that stays structural. `T` must be default-constructible. |
| `ctc::map<K, V, N>` | Fixed-capacity map: unique keys, **insertion order preserved** (like a JSON object), linear heterogeneous lookup. |
| `ctc::pair<F, S>` | A structural `std::pair` (aggregate, two public members). |

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
- `map`: `m[key] = value` inserts like `std::map`; `find`/`contains`/`at`
  accept **any key type comparable with `==`** (a `std::string_view` against
  `ctc::string` keys, for instance); `insert`/`insert_or_assign`/`erase`
  (order-preserving); iteration yields `ctc::pair<K, V> &` in insertion order.

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
