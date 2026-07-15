#ifndef CTC__UTILITY__HPP
#define CTC__UTILITY__HPP

#ifndef CTC_IN_A_MODULE
#include <cstddef>
#include <cstdlib>
#endif

#ifdef CTC_IN_A_MODULE
#define CTC_EXPORT export
#else
#define CTC_EXPORT
#endif

namespace ctc {

using std::size_t;

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

} // namespace detail

} // namespace ctc

#endif
