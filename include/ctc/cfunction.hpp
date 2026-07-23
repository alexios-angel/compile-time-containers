#ifndef CTC__CFUNCTION__HPP
#define CTC__CFUNCTION__HPP

#include "utility.hpp"

#ifndef CTC_IN_A_MODULE
#include <type_traits>
#include <utility>
#endif

// A constexpr type-erased callable - std::function is not usable in
// constant evaluation, this is. The callable sits behind a
// constexpr-virtual interface; copying deep-copies it through a clone
// hook (a function value is a value). Ownership is a hand-rolled
// constexpr new/delete: std::unique_ptr is only constexpr from C++23,
// and ctc's floor is C++20.
//
// (Moved here from compile-time-javascript, where it type-erases the
// interpreter's native functions; compile-time-browser's layout hooks
// ride it too.)

namespace ctc {

CTC_EXPORT template <class> class cfunction;

CTC_EXPORT template <class R, class... Args> class cfunction<R(Args...)> {
public:
	constexpr cfunction() noexcept = default; // empty
	template <class F>
	    requires(!std::is_same_v<std::decay_t<F>, cfunction>)
	constexpr cfunction(F f) : ptr_{new impl<F>{std::move(f)}} { }

	constexpr cfunction(const cfunction & o) : ptr_{o.ptr_ ? o.ptr_->clone() : nullptr} { }
	constexpr cfunction(cfunction && o) noexcept : ptr_{o.ptr_} { o.ptr_ = nullptr; }
	constexpr cfunction & operator=(const cfunction & o) {
		if (this != &o) {
			delete ptr_;
			ptr_ = o.ptr_ ? o.ptr_->clone() : nullptr;
		}
		return *this;
	}
	constexpr cfunction & operator=(cfunction && o) noexcept {
		if (this != &o) {
			delete ptr_;
			ptr_ = o.ptr_;
			o.ptr_ = nullptr;
		}
		return *this;
	}
	constexpr ~cfunction() { delete ptr_; }

	constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }
	constexpr R operator()(Args... args) const { return ptr_->call(static_cast<Args>(args)...); }

private:
	struct iface {
		constexpr virtual R call(Args...) const = 0;
		constexpr virtual iface * clone() const = 0;
		constexpr virtual ~iface() = default;
	};
	template <class F> struct impl final : iface {
		constexpr explicit impl(F f) : f_{std::move(f)} { }
		// explicitly defaulted: gcc defines implicit virtual destructors
		// lazily, which is too late for a constexpr `delete` through the
		// base pointer ("used before its definition")
		constexpr ~impl() override = default;
		constexpr R call(Args... args) const override { return f_(static_cast<Args>(args)...); }
		constexpr iface * clone() const override { return new impl{f_}; }
		F f_;
	};
	iface * ptr_ = nullptr;
};

} // namespace ctc

#endif
