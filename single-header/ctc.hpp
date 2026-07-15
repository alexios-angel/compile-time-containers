/*
MIT License

Copyright (c) 2026 Alexios Angel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef CTC__HPP
#define CTC__HPP

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

#ifndef CTC__PAIR__HPP
#define CTC__PAIR__HPP

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

namespace ctc {

// A structural std::pair: two public members and nothing else, so a
// pair (and any container of pairs) can be a non-type template
// parameter, which std::pair is not guaranteed to support. Aggregate:
// works with designated initializers and structured bindings.
CTC_EXPORT template <typename First, typename Second> struct pair {
	using first_type = First;
	using second_type = Second;

	First first{};
	Second second{};

	friend constexpr bool operator==(const pair &, const pair &) = default;
};

template <typename First, typename Second> pair(First, Second) -> pair<First, Second>;

CTC_EXPORT template <typename First, typename Second> constexpr pair<First, Second> make_pair(First first, Second second) noexcept {
	return {first, second};
}

} // namespace ctc

#endif

#ifndef CTC__STRING__HPP
#define CTC__STRING__HPP

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

#ifndef CTC_IN_A_MODULE
#include <string_view>
#include <type_traits>
#include <iosfwd>
#endif

// A fixed-capacity constexpr string. Unlike ctll::fixed_string (which
// decodes to UTF-32 code points), the content is CharT code units,
// stored as written - so `string<N>` is byte-oriented and converts to
// std::string_view for free.

namespace ctc {

CTC_EXPORT template <typename CharT, size_t N> struct basic_string {
	using value_type = CharT;
	using size_type = size_t;
	using view_type = std::basic_string_view<CharT>;
	using reference = CharT &;
	using const_reference = const CharT &;
	using iterator = CharT *;
	using const_iterator = const CharT *;

	static constexpr size_type npos = static_cast<size_type>(-1);

	// content[size()] is always CharT{} (so c_str() works), and every
	// unit past the end is kept CharT{} by the mutators - so equal
	// contents mean equivalent template arguments when a value is used
	// as an NTTP (template-argument equivalence compares every array
	// element, not just the first size() units).
	CharT content[N + 1]{};
	size_type real_size{0};

	constexpr basic_string() noexcept = default;

	// from a string literal (the terminator is not copied, not counted)
	template <size_t M> requires (M <= N + 1) constexpr basic_string(const CharT (&literal)[M]) noexcept {
		for (size_type i{0}; i + 1 < M; ++i) {
			content[i] = literal[i];
		}
		real_size = M - 1;
	}

	constexpr basic_string(const CharT * from, size_type count) noexcept {
		if (count > N) {
			detail::precondition_violated("ctc::basic_string: content does not fit the capacity");
		}
		for (size_type i{0}; i < count; ++i) {
			content[i] = from[i];
		}
		real_size = count;
	}

	explicit constexpr basic_string(view_type view) noexcept: basic_string{view.data(), view.size()} { }

	constexpr basic_string(size_type count, CharT unit) noexcept {
		if (count > N) {
			detail::precondition_violated("ctc::basic_string: content does not fit the capacity");
		}
		for (size_type i{0}; i < count; ++i) {
			content[i] = unit;
		}
		real_size = count;
	}

	// from another capacity (checked to fit)
	template <size_t M> requires (M != N) explicit constexpr basic_string(const basic_string<CharT, M> & other) noexcept: basic_string{other.data(), other.size()} { }

	// observers
	constexpr size_type size() const noexcept {
		return real_size;
	}
	constexpr size_type length() const noexcept {
		return real_size;
	}
	static constexpr size_type capacity() noexcept {
		return N;
	}
	static constexpr size_type max_size() noexcept {
		return N;
	}
	constexpr bool empty() const noexcept {
		return real_size == 0;
	}
	constexpr bool full() const noexcept {
		return real_size == N;
	}

	// element access
	constexpr CharT * data() noexcept {
		return content;
	}
	constexpr const CharT * data() const noexcept {
		return content;
	}
	constexpr const CharT * c_str() const noexcept {
		return content;
	}
	constexpr reference operator[](size_type i) noexcept {
		if (i >= real_size) {
			detail::precondition_violated("ctc::basic_string: index out of range");
		}
		return content[i];
	}
	constexpr const_reference operator[](size_type i) const noexcept {
		if (i >= real_size) {
			detail::precondition_violated("ctc::basic_string: index out of range");
		}
		return content[i];
	}
	constexpr reference at(size_type i) noexcept {
		return (*this)[i];
	}
	constexpr const_reference at(size_type i) const noexcept {
		return (*this)[i];
	}
	constexpr reference front() noexcept {
		return (*this)[0];
	}
	constexpr const_reference front() const noexcept {
		return (*this)[0];
	}
	constexpr reference back() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::basic_string: back() on an empty string");
		}
		return content[real_size - 1];
	}
	constexpr const_reference back() const noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::basic_string: back() on an empty string");
		}
		return content[real_size - 1];
	}

	// iteration
	constexpr iterator begin() noexcept {
		return content;
	}
	constexpr const_iterator begin() const noexcept {
		return content;
	}
	constexpr iterator end() noexcept {
		return content + real_size;
	}
	constexpr const_iterator end() const noexcept {
		return content + real_size;
	}
	constexpr const_iterator cbegin() const noexcept {
		return begin();
	}
	constexpr const_iterator cend() const noexcept {
		return end();
	}

	// conversion
	constexpr view_type view() const noexcept {
		return view_type{content, real_size};
	}
	constexpr operator view_type() const noexcept {
		return view();
	}

	// mutation (every vacated unit is reset to CharT{}, see `content`)
	constexpr void push_back(CharT unit) noexcept {
		if (real_size == N) {
			detail::precondition_violated("ctc::basic_string: push_back on a full string");
		}
		content[real_size++] = unit;
	}
	constexpr void pop_back() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::basic_string: pop_back on an empty string");
		}
		content[--real_size] = CharT{};
	}
	constexpr void clear() noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = CharT{};
		}
		real_size = 0;
	}
	constexpr void resize(size_type count, CharT unit = CharT{}) noexcept {
		if (count > N) {
			detail::precondition_violated("ctc::basic_string: resize beyond the capacity");
		}
		for (size_type i{real_size}; i < count; ++i) {
			content[i] = unit;
		}
		for (size_type i{count}; i < real_size; ++i) {
			content[i] = CharT{};
		}
		real_size = count;
	}
	constexpr basic_string & append(view_type suffix) noexcept {
		if (suffix.size() > N - real_size) {
			detail::precondition_violated("ctc::basic_string: append beyond the capacity");
		}
		for (size_type i{0}; i < suffix.size(); ++i) {
			content[real_size + i] = suffix[i];
		}
		real_size += suffix.size();
		return *this;
	}
	constexpr basic_string & append(size_type count, CharT unit) noexcept {
		if (count > N - real_size) {
			detail::precondition_violated("ctc::basic_string: append beyond the capacity");
		}
		for (size_type i{0}; i < count; ++i) {
			content[real_size + i] = unit;
		}
		real_size += count;
		return *this;
	}
	constexpr basic_string & operator+=(view_type suffix) noexcept {
		return append(suffix);
	}
	constexpr basic_string & operator+=(CharT unit) noexcept {
		push_back(unit);
		return *this;
	}

	// search (all delegate to the constexpr std::basic_string_view)
	constexpr size_type find(view_type needle, size_type pos = 0) const noexcept {
		return view().find(needle, pos);
	}
	constexpr size_type find(CharT unit, size_type pos = 0) const noexcept {
		return view().find(unit, pos);
	}
	constexpr size_type rfind(view_type needle, size_type pos = npos) const noexcept {
		return view().rfind(needle, pos);
	}
	constexpr size_type rfind(CharT unit, size_type pos = npos) const noexcept {
		return view().rfind(unit, pos);
	}
	constexpr bool contains(view_type needle) const noexcept {
		return find(needle) != npos;
	}
	constexpr bool contains(CharT unit) const noexcept {
		return find(unit) != npos;
	}
	constexpr bool starts_with(view_type prefix) const noexcept {
		return view().starts_with(prefix);
	}
	constexpr bool starts_with(CharT unit) const noexcept {
		return view().starts_with(unit);
	}
	constexpr bool ends_with(view_type suffix) const noexcept {
		return view().ends_with(suffix);
	}
	constexpr bool ends_with(CharT unit) const noexcept {
		return view().ends_with(unit);
	}
	constexpr int compare(view_type rhs) const noexcept {
		return view().compare(rhs);
	}

	// substring, with the capacity computed at compile time
	template <size_t Pos, size_t Count = npos> constexpr auto substr() const noexcept {
		constexpr size_type available_capacity = (Pos < N) ? (N - Pos) : 0;
		constexpr size_type result_capacity = (Count < available_capacity) ? Count : available_capacity;
		basic_string<CharT, result_capacity> result;
		size_type out{0};
		for (size_type i{Pos}; i < real_size && out != result_capacity; ++i, ++out) {
			result.content[out] = content[i];
		}
		result.real_size = out;
		return result;
	}
	// substring as a view into this string (no copy)
	constexpr view_type substr(size_type pos, size_type count = npos) const noexcept {
		if (pos > real_size) {
			detail::precondition_violated("ctc::basic_string: substr position out of range");
		}
		const size_type available = real_size - pos;
		return view_type{content + pos, count < available ? count : available};
	}

	// a copy with a different capacity (checked to fit; shrunk<V>()
	// right-sizes to exactly size())
	template <size_t M> constexpr basic_string<CharT, M> with_capacity() const noexcept {
		if (real_size > M) {
			detail::precondition_violated("ctc::basic_string: content does not fit the new capacity");
		}
		basic_string<CharT, M> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

template <typename CharT, size_t M> basic_string(const CharT (&)[M]) -> basic_string<CharT, M - 1>;

CTC_EXPORT template <size_t N> using string = basic_string<char, N>;
CTC_EXPORT template <size_t N> using wstring = basic_string<wchar_t, N>;
CTC_EXPORT template <size_t N> using u8string = basic_string<char8_t, N>;
CTC_EXPORT template <size_t N> using u16string = basic_string<char16_t, N>;
CTC_EXPORT template <size_t N> using u32string = basic_string<char32_t, N>;

// make_string("literal"): deduction without spelling basic_string
// (alias-template CTAD needs clang 19, so the aliases cannot deduce)
CTC_EXPORT template <typename CharT, size_t M> constexpr auto make_string(const CharT (&literal)[M]) noexcept {
	return basic_string<CharT, M - 1>{literal};
}

// comparison: across capacities, and against anything convertible to a
// view (string literals, std::string_view, std::string). C++20
// synthesizes the reversed and secondary operators.
CTC_EXPORT template <typename CharT, size_t A, size_t B> constexpr bool operator==(const basic_string<CharT, A> & lhs, const basic_string<CharT, B> & rhs) noexcept {
	return lhs.view() == rhs.view();
}
CTC_EXPORT template <typename CharT, size_t A, size_t B> constexpr auto operator<=>(const basic_string<CharT, A> & lhs, const basic_string<CharT, B> & rhs) noexcept {
	return lhs.compare(rhs.view()) <=> 0;
}
CTC_EXPORT template <typename CharT, size_t A> constexpr bool operator==(const basic_string<CharT, A> & lhs, std::type_identity_t<std::basic_string_view<CharT>> rhs) noexcept {
	return lhs.view() == rhs;
}
CTC_EXPORT template <typename CharT, size_t A> constexpr auto operator<=>(const basic_string<CharT, A> & lhs, std::type_identity_t<std::basic_string_view<CharT>> rhs) noexcept {
	return lhs.compare(rhs) <=> 0;
}

// concatenation
CTC_EXPORT template <typename CharT, size_t A, size_t B> constexpr auto operator+(const basic_string<CharT, A> & lhs, const basic_string<CharT, B> & rhs) noexcept {
	basic_string<CharT, A + B> result;
	result.append(lhs.view());
	result.append(rhs.view());
	return result;
}
CTC_EXPORT template <typename CharT, size_t A, size_t M> constexpr auto operator+(const basic_string<CharT, A> & lhs, const CharT (&rhs)[M]) noexcept {
	return lhs + basic_string<CharT, M - 1>{rhs};
}
CTC_EXPORT template <typename CharT, size_t M, size_t B> constexpr auto operator+(const CharT (&lhs)[M], const basic_string<CharT, B> & rhs) noexcept {
	return basic_string<CharT, M - 1>{lhs} + rhs;
}

// iostream interoperability
CTC_EXPORT template <typename CharT, typename Traits, size_t A> std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & stream, const basic_string<CharT, A> & string) {
	return stream << std::basic_string_view<CharT, Traits>{string.data(), string.size()};
}

} // namespace ctc

#endif

#ifndef CTC__VECTOR__HPP
#define CTC__VECTOR__HPP

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

#ifndef CTC_IN_A_MODULE
#include <initializer_list>
#include <type_traits>
#endif

// A fixed-capacity constexpr vector (an inplace_vector that stays a
// structural type). The elements live in a plain public array, so a
// vector built in constant evaluation can be a non-type template
// parameter and can persist to runtime in static storage.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct vector {
	// the whole array exists up front, so elements must themselves be
	// default-constructible (the price of staying a structural type -
	// union tricks would lose NTTP support)
	static_assert(std::is_default_constructible_v<T>, "ctc::vector<T, N> requires a default-constructible T");

	using value_type = T;
	using size_type = size_t;
	using reference = T &;
	using const_reference = const T &;
	using iterator = T *;
	using const_iterator = const T *;

	// every slot past size() is kept equal to T{} by the mutators, so
	// equal contents mean equivalent template arguments when a value is
	// used as an NTTP (template-argument equivalence compares every
	// array element, not just the first size() elements).
	T content[N ? N : 1]{};
	size_type real_size{0};

	constexpr vector() noexcept = default;

	constexpr vector(std::initializer_list<T> init) {
		if (init.size() > N) {
			detail::precondition_violated("ctc::vector: content does not fit the capacity");
		}
		for (const T & element : init) {
			content[real_size++] = element;
		}
	}

	constexpr vector(size_type count, const T & value) {
		if (count > N) {
			detail::precondition_violated("ctc::vector: content does not fit the capacity");
		}
		for (size_type i{0}; i < count; ++i) {
			content[i] = value;
		}
		real_size = count;
	}

	constexpr vector(const T * from, size_type count) {
		if (count > N) {
			detail::precondition_violated("ctc::vector: content does not fit the capacity");
		}
		for (size_type i{0}; i < count; ++i) {
			content[i] = from[i];
		}
		real_size = count;
	}

	// from another capacity (checked to fit)
	template <size_t M> requires (M != N) explicit constexpr vector(const vector<T, M> & other): vector{other.data(), other.size()} { }

	// observers
	constexpr size_type size() const noexcept {
		return real_size;
	}
	static constexpr size_type capacity() noexcept {
		return N;
	}
	static constexpr size_type max_size() noexcept {
		return N;
	}
	constexpr bool empty() const noexcept {
		return real_size == 0;
	}
	constexpr bool full() const noexcept {
		return real_size == N;
	}

	// element access
	constexpr T * data() noexcept {
		return content;
	}
	constexpr const T * data() const noexcept {
		return content;
	}
	constexpr reference operator[](size_type i) noexcept {
		if (i >= real_size) {
			detail::precondition_violated("ctc::vector: index out of range");
		}
		return content[i];
	}
	constexpr const_reference operator[](size_type i) const noexcept {
		if (i >= real_size) {
			detail::precondition_violated("ctc::vector: index out of range");
		}
		return content[i];
	}
	constexpr reference at(size_type i) noexcept {
		return (*this)[i];
	}
	constexpr const_reference at(size_type i) const noexcept {
		return (*this)[i];
	}
	constexpr reference front() noexcept {
		return (*this)[0];
	}
	constexpr const_reference front() const noexcept {
		return (*this)[0];
	}
	constexpr reference back() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::vector: back() on an empty vector");
		}
		return content[real_size - 1];
	}
	constexpr const_reference back() const noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::vector: back() on an empty vector");
		}
		return content[real_size - 1];
	}

	// iteration
	constexpr iterator begin() noexcept {
		return content;
	}
	constexpr const_iterator begin() const noexcept {
		return content;
	}
	constexpr iterator end() noexcept {
		return content + real_size;
	}
	constexpr const_iterator end() const noexcept {
		return content + real_size;
	}
	constexpr const_iterator cbegin() const noexcept {
		return begin();
	}
	constexpr const_iterator cend() const noexcept {
		return end();
	}

	// mutation (every vacated slot is reset to T{}, see `content`)
	constexpr void push_back(const T & value) {
		if (real_size == N) {
			detail::precondition_violated("ctc::vector: push_back on a full vector");
		}
		content[real_size++] = value;
	}
	template <typename... Args> constexpr reference emplace_back(Args &&... args) {
		if (real_size == N) {
			detail::precondition_violated("ctc::vector: emplace_back on a full vector");
		}
		content[real_size] = T{static_cast<Args &&>(args)...};
		return content[real_size++];
	}
	constexpr void pop_back() {
		if (real_size == 0) {
			detail::precondition_violated("ctc::vector: pop_back on an empty vector");
		}
		content[--real_size] = T{};
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = T{};
		}
		real_size = 0;
	}
	constexpr void resize(size_type count, const T & value = T{}) {
		if (count > N) {
			detail::precondition_violated("ctc::vector: resize beyond the capacity");
		}
		for (size_type i{real_size}; i < count; ++i) {
			content[i] = value;
		}
		for (size_type i{count}; i < real_size; ++i) {
			content[i] = T{};
		}
		real_size = count;
	}
	constexpr iterator insert(const_iterator position, const T & value) {
		if (real_size == N) {
			detail::precondition_violated("ctc::vector: insert into a full vector");
		}
		const size_type index = static_cast<size_type>(position - content);
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = value;
		++real_size;
		return content + index;
	}
	constexpr iterator erase(const_iterator position) {
		const size_type index = static_cast<size_type>(position - content);
		if (index >= real_size) {
			detail::precondition_violated("ctc::vector: erase position out of range");
		}
		for (size_type i{index}; i + 1 < real_size; ++i) {
			content[i] = content[i + 1];
		}
		content[--real_size] = T{};
		return content + index;
	}

	// a copy with a different capacity (checked to fit; shrunk<V>()
	// right-sizes to exactly size())
	template <size_t M> constexpr vector<T, M> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::vector: content does not fit the new capacity");
		}
		vector<T, M> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise, across capacities
CTC_EXPORT template <typename T, size_t A, size_t B> constexpr bool operator==(const vector<T, A> & lhs, const vector<T, B> & rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	for (size_t i{0}; i < lhs.size(); ++i) {
		if (!(lhs[i] == rhs[i])) {
			return false;
		}
	}
	return true;
}

} // namespace ctc

#endif

#ifndef CTC__MAP__HPP
#define CTC__MAP__HPP

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

#ifndef CTC__PAIR__HPP
#define CTC__PAIR__HPP

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

namespace ctc {

// A structural std::pair: two public members and nothing else, so a
// pair (and any container of pairs) can be a non-type template
// parameter, which std::pair is not guaranteed to support. Aggregate:
// works with designated initializers and structured bindings.
CTC_EXPORT template <typename First, typename Second> struct pair {
	using first_type = First;
	using second_type = Second;

	First first{};
	Second second{};

	friend constexpr bool operator==(const pair &, const pair &) = default;
};

template <typename First, typename Second> pair(First, Second) -> pair<First, Second>;

CTC_EXPORT template <typename First, typename Second> constexpr pair<First, Second> make_pair(First first, Second second) noexcept {
	return {first, second};
}

} // namespace ctc

#endif

#ifndef CTC_IN_A_MODULE
#include <initializer_list>
#endif

// A fixed-capacity constexpr map: unique keys, insertion order
// preserved (like a JSON object), linear lookups. At compile-time
// sizes linear beats the bookkeeping of staying sorted, and keeping
// the elements a plain public array of ctc::pair keeps the whole map a
// structural type - usable as a non-type template parameter.
//
// Lookups are heterogeneous: any key type comparable to Key with ==
// works (a std::string_view against ctc::string keys, for instance).

namespace ctc {

CTC_EXPORT template <typename Key, typename Value, size_t N> struct map {
	using key_type = Key;
	using mapped_type = Value;
	using value_type = pair<Key, Value>;
	using size_type = size_t;
	using reference = value_type &;
	using const_reference = const value_type &;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	// every slot past size() is kept equal to value_type{} by the
	// mutators, so equal contents mean equivalent template arguments
	// when a value is used as an NTTP
	value_type content[N ? N : 1]{};
	size_type real_size{0};

	constexpr map() noexcept = default;

	constexpr map(std::initializer_list<value_type> init) {
		for (const value_type & element : init) {
			insert_or_assign(element.first, element.second);
		}
	}

	// observers
	constexpr size_type size() const noexcept {
		return real_size;
	}
	static constexpr size_type capacity() noexcept {
		return N;
	}
	static constexpr size_type max_size() noexcept {
		return N;
	}
	constexpr bool empty() const noexcept {
		return real_size == 0;
	}
	constexpr bool full() const noexcept {
		return real_size == N;
	}

	// iteration, in insertion order
	constexpr iterator begin() noexcept {
		return content;
	}
	constexpr const_iterator begin() const noexcept {
		return content;
	}
	constexpr iterator end() noexcept {
		return content + real_size;
	}
	constexpr const_iterator end() const noexcept {
		return content + real_size;
	}
	constexpr const_iterator cbegin() const noexcept {
		return begin();
	}
	constexpr const_iterator cend() const noexcept {
		return end();
	}
	constexpr value_type * data() noexcept {
		return content;
	}
	constexpr const value_type * data() const noexcept {
		return content;
	}

	// lookup
	template <typename K> constexpr iterator find(const K & key) noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (content[i].first == key) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (content[i].first == key) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr Value & at(const K & key) noexcept {
		const iterator position = find(key);
		if (position == end()) {
			detail::precondition_violated("ctc::map: at() with a key that is not in the map");
		}
		return position->second;
	}
	template <typename K> constexpr const Value & at(const K & key) const noexcept {
		const const_iterator position = find(key);
		if (position == end()) {
			detail::precondition_violated("ctc::map: at() with a key that is not in the map");
		}
		return position->second;
	}
	// like std::map, m[key] inserts a default-constructed value for a
	// missing key - the natural way to build a map in a constexpr
	// function: m["answer"] = 42;
	template <typename K> constexpr Value & operator[](const K & key) {
		const iterator position = find(key);
		if (position != end()) {
			return position->second;
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::map: insertion into a full map");
		}
		content[real_size].first = Key{key};
		return content[real_size++].second;
	}

	// insertion
	template <typename K> constexpr pair<iterator, bool> insert_or_assign(const K & key, const Value & value) {
		if (const iterator position = find(key); position != end()) {
			position->second = value;
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::map: insertion into a full map");
		}
		content[real_size] = value_type{Key{key}, value};
		return {content + real_size++, true};
	}
	// like std::map::insert, an existing key is left untouched
	constexpr pair<iterator, bool> insert(const value_type & element) {
		if (const iterator position = find(element.first); position != end()) {
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::map: insertion into a full map");
		}
		content[real_size] = element;
		return {content + real_size++, true};
	}

	// removal (later elements shift down; insertion order is preserved,
	// the vacated slot is reset to value_type{})
	template <typename K> constexpr size_type erase(const K & key) {
		const iterator position = find(key);
		if (position == end()) {
			return 0;
		}
		const size_type index = static_cast<size_type>(position - content);
		for (size_type i{index}; i + 1 < real_size; ++i) {
			content[i] = content[i + 1];
		}
		content[--real_size] = value_type{};
		return 1;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = value_type{};
		}
		real_size = 0;
	}

	// a copy with a different capacity (checked to fit; shrunk<V>()
	// right-sizes to exactly size())
	template <size_t M> constexpr map<Key, Value, M> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::map: content does not fit the new capacity");
		}
		map<Key, Value, M> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise in insertion order, across capacities (two
// maps with the same entries inserted in a different order are NOT
// equal - the map is ordered, like the JSON objects it models)
CTC_EXPORT template <typename Key, typename Value, size_t A, size_t B> constexpr bool operator==(const map<Key, Value, A> & lhs, const map<Key, Value, B> & rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	for (size_t i{0}; i < lhs.size(); ++i) {
		if (!(lhs.content[i] == rhs.content[i])) {
			return false;
		}
	}
	return true;
}

} // namespace ctc

#endif

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
