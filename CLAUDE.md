# ctc — compile-time containers

Header-only C++20 library (namespace `ctc`): fixed-capacity constexpr
`basic_string`/`vector`/`map`/`pair` that are STRUCTURAL types (usable as
NTTPs, persistable to runtime static storage). This is the shared containers
layer for the compile-time-* family and (eventually) notre. Repo:
github.com/alexios-angel/compile-time-containers — work on `main`. License:
MIT (original code, unlike the Apache-2.0 CTRE-derived siblings).

Prefer ripgrep (`rg`) over `grep`.

## Build & test (compiling IS the test — suites are `static_assert`s)
```bash
make                # C++20, PCH of ctc.hpp, then every tests/*.cpp -> .o
make CXX=clang++
make clean
make single-header  # quom amalgam + LICENSE prepend -> single-header/ctc.hpp
cmake -B build && cmake --build build -j && ctest --test-dir build
cd examples && make run
```
Flags are fixed: `-std=c++20 -Iinclude -O2 -pedantic -Wall -Wextra -Werror
-Wconversion` — keep code warning-clean. No raised constexpr limits needed
(unlike the Earley siblings). CI: gcc 12–14, clang 15/16/18, C++20 only.

## Layout
- `include/ctc.hpp` — umbrella; doc comment + `shrunk<V>()` live here.
- `include/ctc/` — `utility.hpp` (`CTC_EXPORT`, the precondition trap),
  `pair.hpp`, `string.hpp`, `vector.hpp`, `map.hpp`. Self-contained, zero
  dependencies — vendorable as a unit like ctll/ctlark are.
- `tests/` — one static_assert suite per container (`int main() { }` at the
  bottom, family style). `examples/config.cpp`. `ctc.cppm` (C++23 module).

## Load-bearing invariants
- **Structural**: public `content[]` array + public `real_size`, no unions,
  nothing private. `vector<T, N>` therefore requires default-constructible T.
- **Slot reset**: every mutator that vacates a slot (`pop_back`, `erase`,
  `clear`, `resize`, string mutators) resets it to `T{}`/`CharT{}` so
  template-argument equivalence (which compares the WHOLE array) matches
  logical equality. Tests pin this with `tag<direct>` vs `tag<detoured>`
  `is_same` checks — do not remove the resets.
- **String storage**: `CharT content[N + 1]` — `content[size()]` is always 0
  (`c_str()` works). Code units as written; NOT ctll::fixed_string's UTF-32.
- **Preconditions** call `ctc::detail::precondition_violated(msg)` — a
  non-constexpr function: compile error in constant evaluation (message shows
  in the backtrace), `std::abort()` at runtime. No exceptions anywhere.
- **map** is insertion-ordered with linear heterogeneous lookup (any key
  comparable via `==`); equality is order-sensitive. `operator[]` inserts
  like std::map.
- `shrunk<V>()` = `V.with_capacity<V.size()>()` — the oversize-then-shrink
  idiom every consumer of this library builds on.

## Conventions
Tabs, K&R braces, `constexpr`/`noexcept` (mutators that can trap still say
noexcept — the trap aborts, it does not throw), `CTC_EXPORT` on every public
name, stdlib includes guarded by `#ifndef CTC_IN_A_MODULE`, include guards
`CTC__<NAME>__HPP`. Avoid alias-template CTAD (`ctc::string{"x"}`) in tests
and examples — it needs clang ≥ 19; use `ctc::basic_string{"x"}` or
`make_string`.

## Gotchas
- clangd parses these headers in pre-C++20 mode and reports FALSE errors
  (requires-clauses, `<=>`, char8_t, consteval); trust the real g++/clang
  build, not clangd.
- `tests/*.cpp` include `<ctc.hpp>` via `-Iinclude`; keep new test files in
  that form so the PCH and single-header stay honest.
