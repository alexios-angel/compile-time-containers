#ifndef CTC__HPP
#define CTC__HPP

#include "ctc/utility.hpp"
#include "ctc/pair.hpp"
#include "ctc/string.hpp"
#include "ctc/vector.hpp"
#include "ctc/map.hpp"

// ctc: compile-time containers. Fixed-capacity constexpr strings,
// vectors and maps that stay STRUCTURAL types - values built at
// compile time can be non-type template parameters and can persist to
// runtime in static storage:
//
//   constexpr auto greeting = [] {
//       ctc::string<32> s;
//       s.append("Hello, ");
//       s.append("World!");
//       return s;
//   }();
//   static_assert(greeting == "Hello, World!");
//
// The capacity is a template parameter (constexpr allocation cannot
// escape to runtime in C++20). When the final size is only known
// after building, use the oversize-then-shrink two-step: build in a
// generous buffer, then right-size it with shrunk<V>() - the value
// keeps only the storage it needs:
//
//   constexpr auto big = build();              // ctc::vector<int, 1024>
//   constexpr auto v = ctc::shrunk<big>();     // ctc::vector<int, v.size()>
//
// Preconditions (overflowing the capacity, indexing out of range) call
// a non-constexpr trap: in constant evaluation that is a compile
// error pointing at the violation, at runtime it aborts. The library
// is exception-free and warning-clean under -Wall -Wextra -Wconversion.

namespace ctc {

// the "oversize, then right-size" two-step: a copy of V whose capacity
// is exactly V.size() (V must be a constant - pass the container as a
// template argument, which its structurality allows)
CTC_EXPORT template <auto V> consteval auto shrunk() noexcept {
	return V.template with_capacity<V.size()>();
}

} // namespace ctc

#endif
