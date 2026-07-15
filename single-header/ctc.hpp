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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

#ifndef CTC__ARRAY__HPP
#define CTC__ARRAY__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

} // namespace ctc

#endif

// A structural std::array: exactly N elements, aggregate (brace
// elision works: ctc::array<int, 3>{1, 2, 3}), guaranteed usable as a
// non-type template parameter. std::array is usually structural too -
// this one is structural BY CONTRACT, like the rest of the library.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct array {
	using value_type = T;
	using size_type = size_t;
	using reference = T &;
	using const_reference = const T &;
	using iterator = T *;
	using const_iterator = const T *;

	T content[N ? N : 1]{};

	// observers
	static constexpr size_type size() noexcept {
		return N;
	}
	static constexpr size_type max_size() noexcept {
		return N;
	}
	static constexpr bool empty() noexcept {
		return N == 0;
	}

	// element access
	constexpr T * data() noexcept {
		return content;
	}
	constexpr const T * data() const noexcept {
		return content;
	}
	constexpr reference operator[](size_type i) noexcept {
		if (i >= N) {
			detail::precondition_violated("ctc::array: index out of range");
		}
		return content[i];
	}
	constexpr const_reference operator[](size_type i) const noexcept {
		if (i >= N) {
			detail::precondition_violated("ctc::array: index out of range");
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
		return (*this)[N - 1];
	}
	constexpr const_reference back() const noexcept {
		return (*this)[N - 1];
	}

	// iteration
	constexpr iterator begin() noexcept {
		return content;
	}
	constexpr const_iterator begin() const noexcept {
		return content;
	}
	constexpr iterator end() noexcept {
		return content + N;
	}
	constexpr const_iterator end() const noexcept {
		return content + N;
	}
	constexpr const_iterator cbegin() const noexcept {
		return begin();
	}
	constexpr const_iterator cend() const noexcept {
		return end();
	}

	constexpr void fill(const T & value) {
		for (size_type i{0}; i < N; ++i) {
			content[i] = value;
		}
	}

	friend constexpr bool operator==(const array &, const array &) = default;
};

template <typename T, typename... U> array(T, U...) -> array<T, 1 + sizeof...(U)>;

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

#ifndef CTC__DEQUE__HPP
#define CTC__DEQUE__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// A fixed-capacity constexpr deque. NOT a ring buffer: the front is
// always content[0], because a ring's head offset would make equal
// contents different template arguments (NTTP identity is layout
// identity). The price is O(size) front operations - irrelevant at
// compile-time sizes; the payoff is contiguous iteration and the same
// canonical-layout guarantee as every other ctc container. Publicly
// derives from ctc::vector (public bases keep a type structural), so
// the whole vector API is available.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct deque : vector<T, N> {
	using vector<T, N>::vector;

	constexpr void push_front(const T & value) {
		if (this->size() == N) {
			detail::precondition_violated("ctc::deque: push_front on a full deque");
		}
		this->insert(this->begin(), value);
	}
	constexpr void pop_front() {
		if (this->empty()) {
			detail::precondition_violated("ctc::deque: pop_front on an empty deque");
		}
		this->erase(this->begin());
	}

	// a copy with a different capacity (shadows vector's, so shrunk<V>()
	// on a deque stays a deque)
	template <size_t M> constexpr deque<T, M> with_capacity() const {
		if (this->size() > M) {
			detail::precondition_violated("ctc::deque: content does not fit the new capacity");
		}
		deque<T, M> result;
		for (size_t i{0}; i < this->size(); ++i) {
			result.content[i] = this->content[i];
		}
		result.real_size = this->size();
		return result;
	}
};

} // namespace ctc

#endif

#ifndef CTC__FORWARD_LIST__HPP
#define CTC__FORWARD_LIST__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

} // namespace ctc

#endif

#ifndef CTC_IN_A_MODULE
#include <type_traits>
#endif

// A fixed-capacity constexpr singly-linked list (std::forward_list):
// nodes in a plain array linked by INDICES (pointers into your own
// members are not structural-friendly and would dangle on copy).
// Iterators stay valid across other insertions/erasures, which is the
// point of a list.
//
// CAVEAT (the one container family where this is unavoidable): node
// placement depends on the operation history, so equal contents are
// NOT guaranteed to be equivalent template arguments - only == is
// content-based. with_capacity() (and so shrunk<V>()) rebuilds
// compactly and canonically: shrink before using a list as an NTTP if
// identity matters.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct forward_list {
	static_assert(std::is_default_constructible_v<T>, "ctc::forward_list<T, N> requires a default-constructible T");

	using value_type = T;
	using size_type = size_t;

	static constexpr size_type nil = static_cast<size_type>(-1);

	struct node {
		T value{};
		size_type next{nil};
	};

	// vacated nodes are reset to T{} and threaded on the free list
	node content[N ? N : 1]{};
	size_type head{nil};
	size_type free_head{0};
	size_type real_size{0};

	constexpr forward_list() noexcept {
		for (size_type i{0}; i < N; ++i) {
			content[i].next = i + 1 < N ? i + 1 : nil;
		}
		if (N == 0) {
			free_head = nil;
		}
	}

	struct const_iterator {
		const forward_list * list{nullptr};
		size_type index{nil};

		constexpr const T & operator*() const noexcept {
			return list->content[index].value;
		}
		constexpr const T * operator->() const noexcept {
			return &list->content[index].value;
		}
		constexpr const_iterator & operator++() noexcept {
			index = list->content[index].next;
			return *this;
		}
		constexpr const_iterator operator++(int) noexcept {
			const_iterator before = *this;
			++*this;
			return before;
		}
		friend constexpr bool operator==(const const_iterator &, const const_iterator &) noexcept = default;
	};
	struct iterator {
		forward_list * list{nullptr};
		size_type index{nil};

		constexpr T & operator*() const noexcept {
			return list->content[index].value;
		}
		constexpr T * operator->() const noexcept {
			return &list->content[index].value;
		}
		constexpr iterator & operator++() noexcept {
			index = list->content[index].next;
			return *this;
		}
		constexpr iterator operator++(int) noexcept {
			iterator before = *this;
			++*this;
			return before;
		}
		constexpr operator const_iterator() const noexcept {
			return {list, index};
		}
		friend constexpr bool operator==(const iterator &, const iterator &) noexcept = default;
	};

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

	constexpr iterator begin() noexcept {
		return {this, head};
	}
	constexpr const_iterator begin() const noexcept {
		return {this, head};
	}
	constexpr iterator end() noexcept {
		return {this, nil};
	}
	constexpr const_iterator end() const noexcept {
		return {this, nil};
	}

	constexpr T & front() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::forward_list: front() on an empty list");
		}
		return content[head].value;
	}
	constexpr const T & front() const noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::forward_list: front() on an empty list");
		}
		return content[head].value;
	}

	// mutation
	constexpr void push_front(const T & value) {
		const size_type index = allocate("ctc::forward_list: push_front on a full list");
		content[index].value = value;
		content[index].next = head;
		head = index;
	}
	constexpr void pop_front() {
		if (real_size == 0) {
			detail::precondition_violated("ctc::forward_list: pop_front on an empty list");
		}
		const size_type index = head;
		head = content[index].next;
		release(index);
	}
	// inserts after `position` (std::forward_list semantics); returns
	// an iterator to the new element
	constexpr iterator insert_after(const_iterator position, const T & value) {
		const size_type index = allocate("ctc::forward_list: insert_after on a full list");
		content[index].value = value;
		content[index].next = content[position.index].next;
		content[position.index].next = index;
		return {this, index};
	}
	// erases the element after `position`; returns an iterator to the
	// one after the erased element
	constexpr iterator erase_after(const_iterator position) {
		const size_type victim = content[position.index].next;
		if (victim == nil) {
			detail::precondition_violated("ctc::forward_list: erase_after with nothing after");
		}
		content[position.index].next = content[victim].next;
		const size_type following = content[victim].next;
		release(victim);
		return {this, following};
	}
	constexpr void clear() {
		while (real_size != 0) {
			pop_front();
		}
	}

	// a compact, canonically-laid-out copy (this is also how a list
	// becomes NTTP-identity-safe)
	template <size_t M> constexpr forward_list<T, M> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::forward_list: content does not fit the new capacity");
		}
		forward_list<T, M> result;
		size_type out{0};
		for (size_type i{head}; i != nil; i = content[i].next, ++out) {
			result.content[out].value = content[i].value;
			result.content[out].next = content[i].next != nil ? out + 1 : forward_list<T, M>::nil;
		}
		result.head = real_size != 0 ? 0 : forward_list<T, M>::nil;
		result.free_head = real_size < M ? real_size : forward_list<T, M>::nil;
		result.real_size = real_size;
		return result;
	}

private:
	constexpr size_type allocate(const char * full_message) {
		if (free_head == nil) {
			detail::precondition_violated(full_message);
		}
		const size_type index = free_head;
		free_head = content[index].next;
		++real_size;
		return index;
	}
	constexpr void release(size_type index) {
		content[index].value = T{};
		content[index].next = free_head;
		free_head = index;
		--real_size;
	}
};

// equality: element-wise in list order, across capacities
CTC_EXPORT template <typename T, size_t A, size_t B> constexpr bool operator==(const forward_list<T, A> & lhs, const forward_list<T, B> & rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	auto left = lhs.begin();
	auto right = rhs.begin();
	for (; left != lhs.end(); ++left, ++right) {
		if (!(*left == *right)) {
			return false;
		}
	}
	return true;
}

} // namespace ctc

#endif

#ifndef CTC__LIST__HPP
#define CTC__LIST__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

} // namespace ctc

#endif

#ifndef CTC_IN_A_MODULE
#include <type_traits>
#endif

// A fixed-capacity constexpr doubly-linked list (std::list): nodes in
// a plain array linked by INDICES, same design and same NTTP caveat as
// forward_list.hpp - node placement depends on the operation history,
// so only == is content-based; with_capacity()/shrunk<V>() rebuilds
// canonically.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct list {
	static_assert(std::is_default_constructible_v<T>, "ctc::list<T, N> requires a default-constructible T");

	using value_type = T;
	using size_type = size_t;

	static constexpr size_type nil = static_cast<size_type>(-1);

	struct node {
		T value{};
		size_type prev{nil};
		size_type next{nil};
	};

	// vacated nodes are reset to T{} and threaded on the free list
	// (through next)
	node content[N ? N : 1]{};
	size_type head{nil};
	size_type tail{nil};
	size_type free_head{0};
	size_type real_size{0};

	constexpr list() noexcept {
		for (size_type i{0}; i < N; ++i) {
			content[i].next = i + 1 < N ? i + 1 : nil;
		}
		if (N == 0) {
			free_head = nil;
		}
	}

	struct const_iterator {
		const list * container{nullptr};
		size_type index{nil};

		constexpr const T & operator*() const noexcept {
			return container->content[index].value;
		}
		constexpr const T * operator->() const noexcept {
			return &container->content[index].value;
		}
		constexpr const_iterator & operator++() noexcept {
			index = container->content[index].next;
			return *this;
		}
		constexpr const_iterator operator++(int) noexcept {
			const_iterator before = *this;
			++*this;
			return before;
		}
		// --end() is the last element, like std
		constexpr const_iterator & operator--() noexcept {
			index = index == nil ? container->tail : container->content[index].prev;
			return *this;
		}
		constexpr const_iterator operator--(int) noexcept {
			const_iterator before = *this;
			--*this;
			return before;
		}
		friend constexpr bool operator==(const const_iterator &, const const_iterator &) noexcept = default;
	};
	struct iterator {
		list * container{nullptr};
		size_type index{nil};

		constexpr T & operator*() const noexcept {
			return container->content[index].value;
		}
		constexpr T * operator->() const noexcept {
			return &container->content[index].value;
		}
		constexpr iterator & operator++() noexcept {
			index = container->content[index].next;
			return *this;
		}
		constexpr iterator operator++(int) noexcept {
			iterator before = *this;
			++*this;
			return before;
		}
		constexpr iterator & operator--() noexcept {
			index = index == nil ? container->tail : container->content[index].prev;
			return *this;
		}
		constexpr iterator operator--(int) noexcept {
			iterator before = *this;
			--*this;
			return before;
		}
		constexpr operator const_iterator() const noexcept {
			return {container, index};
		}
		friend constexpr bool operator==(const iterator &, const iterator &) noexcept = default;
	};

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

	constexpr iterator begin() noexcept {
		return {this, head};
	}
	constexpr const_iterator begin() const noexcept {
		return {this, head};
	}
	constexpr iterator end() noexcept {
		return {this, nil};
	}
	constexpr const_iterator end() const noexcept {
		return {this, nil};
	}

	constexpr T & front() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: front() on an empty list");
		}
		return content[head].value;
	}
	constexpr const T & front() const noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: front() on an empty list");
		}
		return content[head].value;
	}
	constexpr T & back() noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: back() on an empty list");
		}
		return content[tail].value;
	}
	constexpr const T & back() const noexcept {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: back() on an empty list");
		}
		return content[tail].value;
	}

	// mutation
	constexpr void push_front(const T & value) {
		insert(begin(), value);
	}
	constexpr void push_back(const T & value) {
		insert(end(), value);
	}
	constexpr void pop_front() {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: pop_front on an empty list");
		}
		erase(begin());
	}
	constexpr void pop_back() {
		if (real_size == 0) {
			detail::precondition_violated("ctc::list: pop_back on an empty list");
		}
		erase({this, tail});
	}
	// inserts before `position` (std::list semantics); returns an
	// iterator to the new element
	constexpr iterator insert(const_iterator position, const T & value) {
		if (free_head == nil) {
			detail::precondition_violated("ctc::list: insertion into a full list");
		}
		const size_type index = free_head;
		free_head = content[index].next;
		++real_size;
		content[index].value = value;
		const size_type after = position.index;
		const size_type before = after == nil ? tail : content[after].prev;
		content[index].prev = before;
		content[index].next = after;
		if (before == nil) {
			head = index;
		} else {
			content[before].next = index;
		}
		if (after == nil) {
			tail = index;
		} else {
			content[after].prev = index;
		}
		return {this, index};
	}
	// erases `position`; returns an iterator to the element after it
	constexpr iterator erase(const_iterator position) {
		if (position.index == nil) {
			detail::precondition_violated("ctc::list: erase(end())");
		}
		const size_type index = position.index;
		const size_type before = content[index].prev;
		const size_type after = content[index].next;
		if (before == nil) {
			head = after;
		} else {
			content[before].next = after;
		}
		if (after == nil) {
			tail = before;
		} else {
			content[after].prev = before;
		}
		content[index].value = T{};
		content[index].prev = nil;
		content[index].next = free_head;
		free_head = index;
		--real_size;
		return {this, after};
	}
	constexpr void clear() {
		while (real_size != 0) {
			pop_front();
		}
	}

	// a compact, canonically-laid-out copy (this is also how a list
	// becomes NTTP-identity-safe)
	template <size_t M> constexpr list<T, M> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::list: content does not fit the new capacity");
		}
		list<T, M> result;
		for (size_type i{head}; i != nil; i = content[i].next) {
			result.push_back(content[i].value);
		}
		return result;
	}
};

// equality: element-wise in list order, across capacities
CTC_EXPORT template <typename T, size_t A, size_t B> constexpr bool operator==(const list<T, A> & lhs, const list<T, B> & rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	auto left = lhs.begin();
	auto right = rhs.begin();
	for (; left != lhs.end(); ++left, ++right) {
		if (!(*left == *right)) {
			return false;
		}
	}
	return true;
}

} // namespace ctc

#endif

#ifndef CTC__SET__HPP
#define CTC__SET__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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
#include <type_traits>
#endif

// Fixed-capacity constexpr sorted sets (std::set / std::multiset).
// A plain public array kept sorted by Compare - binary-search lookups,
// heterogeneous (any key comparable through the transparent Compare).
// Because the layout is canonical for a set (sorted, unique), equal
// contents mean equivalent template arguments no matter the insertion
// order. Compare must be stateless: it is constructed on use, not
// stored. Elements are immutable through iterators, like std.

namespace ctc {

namespace detail {

struct key_is_the_element {
	template <typename T> constexpr const T & operator()(const T & element) const noexcept {
		return element;
	}
};

} // namespace detail

CTC_EXPORT template <typename Key, size_t N, typename Compare = less> struct set {
	static_assert(std::is_default_constructible_v<Key>, "ctc::set<Key, N> requires a default-constructible Key");
	static_assert(std::is_empty_v<Compare>, "ctc::set: Compare must be stateless (it is not stored)");

	using key_type = Key;
	using value_type = Key;
	using size_type = size_t;
	using key_compare = Compare;
	using iterator = const Key *; // set elements are immutable, like std
	using const_iterator = const Key *;

	// every slot past size() is kept equal to Key{} by the mutators
	Key content[N ? N : 1]{};
	size_type real_size{0};

	constexpr set() noexcept = default;

	constexpr set(std::initializer_list<Key> init) {
		for (const Key & key : init) {
			insert(key);
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

	// iteration, in sorted order
	constexpr const_iterator begin() const noexcept {
		return content;
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
	constexpr const Key * data() const noexcept {
		return content;
	}

	// lookup
	template <typename K> constexpr const_iterator lower_bound(const K & key) const noexcept {
		return content + detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
	}
	template <typename K> constexpr const_iterator upper_bound(const K & key) const noexcept {
		return content + detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
	}
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		return {lower_bound(key), upper_bound(key)};
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		const const_iterator position = lower_bound(key);
		if (position != end() && !Compare{}(key, *position)) {
			return position;
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return static_cast<size_type>(upper_bound(key) - lower_bound(key));
	}

	// insertion (an existing key is left untouched, like std::set)
	constexpr pair<const_iterator, bool> insert(const Key & key) {
		const size_type index = detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
		if (index < real_size && !Compare{}(key, content[index])) {
			return {content + index, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::set: insertion into a full set");
		}
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = key;
		++real_size;
		return {content + index, true};
	}

	// removal (the vacated slot is reset to Key{})
	template <typename K> constexpr size_type erase(const K & key) {
		const size_type index = detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
		if (index == real_size || Compare{}(key, content[index])) {
			return 0;
		}
		for (size_type i{index}; i + 1 < real_size; ++i) {
			content[i] = content[i + 1];
		}
		content[--real_size] = Key{};
		return 1;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size = 0;
	}

	// a copy with a different capacity (checked to fit)
	template <size_t M> constexpr set<Key, M, Compare> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::set: content does not fit the new capacity");
		}
		set<Key, M, Compare> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// std::multiset: duplicate keys allowed; a new equal key is placed
// after the existing ones (stable), so the layout is deterministic
// for a given insertion sequence.
CTC_EXPORT template <typename Key, size_t N, typename Compare = less> struct multiset {
	static_assert(std::is_default_constructible_v<Key>, "ctc::multiset<Key, N> requires a default-constructible Key");
	static_assert(std::is_empty_v<Compare>, "ctc::multiset: Compare must be stateless (it is not stored)");

	using key_type = Key;
	using value_type = Key;
	using size_type = size_t;
	using key_compare = Compare;
	using iterator = const Key *;
	using const_iterator = const Key *;

	Key content[N ? N : 1]{};
	size_type real_size{0};

	constexpr multiset() noexcept = default;

	constexpr multiset(std::initializer_list<Key> init) {
		for (const Key & key : init) {
			insert(key);
		}
	}

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

	constexpr const_iterator begin() const noexcept {
		return content;
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
	constexpr const Key * data() const noexcept {
		return content;
	}

	template <typename K> constexpr const_iterator lower_bound(const K & key) const noexcept {
		return content + detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
	}
	template <typename K> constexpr const_iterator upper_bound(const K & key) const noexcept {
		return content + detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
	}
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		return {lower_bound(key), upper_bound(key)};
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		const const_iterator position = lower_bound(key);
		if (position != end() && !Compare{}(key, *position)) {
			return position;
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return static_cast<size_type>(upper_bound(key) - lower_bound(key));
	}

	constexpr const_iterator insert(const Key & key) {
		if (real_size == N) {
			detail::precondition_violated("ctc::multiset: insertion into a full multiset");
		}
		const size_type index = detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = key;
		++real_size;
		return content + index;
	}

	// removal erases the whole equal range, like std::multiset
	template <typename K> constexpr size_type erase(const K & key) {
		const size_type from = detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
		const size_type upto = detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_the_element{});
		const size_type removed = upto - from;
		for (size_type i{from}; i + removed < real_size; ++i) {
			content[i] = content[i + removed];
		}
		for (size_type i{real_size - removed}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size -= removed;
		return removed;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size = 0;
	}

	template <size_t M> constexpr multiset<Key, M, Compare> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::multiset: content does not fit the new capacity");
		}
		multiset<Key, M, Compare> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise in sorted order, across capacities
CTC_EXPORT template <typename Key, size_t A, size_t B, typename Compare> constexpr bool operator==(const set<Key, A, Compare> & lhs, const set<Key, B, Compare> & rhs) {
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
CTC_EXPORT template <typename Key, size_t A, size_t B, typename Compare> constexpr bool operator==(const multiset<Key, A, Compare> & lhs, const multiset<Key, B, Compare> & rhs) {
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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
#include <type_traits>
#endif

// Fixed-capacity constexpr sorted maps (std::map / std::multimap).
// A plain public array of ctc::pair kept sorted by Compare on the key:
// binary-search lookups, heterogeneous (any key comparable through the
// transparent Compare). Because the layout of a map is canonical
// (sorted, unique keys), equal contents mean equivalent template
// arguments no matter the insertion order. Compare must be stateless:
// it is constructed on use, not stored.
//
// For the insertion-ordered map (a JSON object), see unordered_map.hpp.

namespace ctc {

namespace detail {

struct key_is_first {
	template <typename P> constexpr const auto & operator()(const P & element) const noexcept {
		return element.first;
	}
};

} // namespace detail

CTC_EXPORT template <typename Key, typename Value, size_t N, typename Compare = less> struct map {
	static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>, "ctc::map<Key, Value, N> requires default-constructible Key and Value");
	static_assert(std::is_empty_v<Compare>, "ctc::map: Compare must be stateless (it is not stored)");

	using key_type = Key;
	using mapped_type = Value;
	using value_type = pair<Key, Value>;
	using size_type = size_t;
	using key_compare = Compare;
	using reference = value_type &;
	using const_reference = const value_type &;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	// every slot past size() is kept equal to value_type{} by the
	// mutators; do not write through an iterator's .first (sortedness
	// and NTTP identity depend on it)
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

	// iteration, in sorted key order
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
	template <typename K> constexpr iterator lower_bound(const K & key) noexcept {
		return content + detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr const_iterator lower_bound(const K & key) const noexcept {
		return content + detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr iterator upper_bound(const K & key) noexcept {
		return content + detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr const_iterator upper_bound(const K & key) const noexcept {
		return content + detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr iterator find(const K & key) noexcept {
		const iterator position = lower_bound(key);
		if (position != end() && !Compare{}(key, position->first)) {
			return position;
		}
		return end();
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		const const_iterator position = lower_bound(key);
		if (position != end() && !Compare{}(key, position->first)) {
			return position;
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return find(key) != end() ? 1 : 0;
	}
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		return {lower_bound(key), upper_bound(key)};
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
	// missing key: m["answer"] = 42;
	template <typename K> constexpr Value & operator[](const K & key) {
		return try_emplace_impl(key)->second;
	}

	// insertion
	template <typename K> constexpr pair<iterator, bool> insert_or_assign(const K & key, const Value & value) {
		const size_type before = real_size;
		const iterator position = try_emplace_impl(key);
		const bool inserted = real_size != before;
		position->second = value;
		return {position, inserted};
	}
	// like std::map::insert, an existing key is left untouched
	constexpr pair<iterator, bool> insert(const value_type & element) {
		const size_type before = real_size;
		const iterator position = try_emplace_impl(element.first);
		const bool inserted = real_size != before;
		if (inserted) {
			position->second = element.second;
		}
		return {position, inserted};
	}
	template <typename K> constexpr pair<iterator, bool> try_emplace(const K & key, const Value & value) {
		const size_type before = real_size;
		const iterator position = try_emplace_impl(key);
		const bool inserted = real_size != before;
		if (inserted) {
			position->second = value;
		}
		return {position, inserted};
	}

	// removal (slots shift down; the vacated slot is reset)
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

	// a copy with a different capacity (checked to fit)
	template <size_t M> constexpr map<Key, Value, M, Compare> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::map: content does not fit the new capacity");
		}
		map<Key, Value, M, Compare> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}

private:
	// the position of `key`, inserting {Key{key}, Value{}} at the
	// sorted position if missing
	template <typename K> constexpr iterator try_emplace_impl(const K & key) {
		const size_type index = detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
		if (index < real_size && !Compare{}(key, content[index].first)) {
			return content + index;
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::map: insertion into a full map");
		}
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = value_type{Key{key}, Value{}};
		++real_size;
		return content + index;
	}
};

// std::multimap: duplicate keys allowed; a new equal key is placed
// after the existing ones (stable). No operator[] / at / insert_or_assign.
CTC_EXPORT template <typename Key, typename Value, size_t N, typename Compare = less> struct multimap {
	static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>, "ctc::multimap<Key, Value, N> requires default-constructible Key and Value");
	static_assert(std::is_empty_v<Compare>, "ctc::multimap: Compare must be stateless (it is not stored)");

	using key_type = Key;
	using mapped_type = Value;
	using value_type = pair<Key, Value>;
	using size_type = size_t;
	using key_compare = Compare;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	value_type content[N ? N : 1]{};
	size_type real_size{0};

	constexpr multimap() noexcept = default;

	constexpr multimap(std::initializer_list<value_type> init) {
		for (const value_type & element : init) {
			insert(element);
		}
	}

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

	template <typename K> constexpr const_iterator lower_bound(const K & key) const noexcept {
		return content + detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr const_iterator upper_bound(const K & key) const noexcept {
		return content + detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
	}
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		return {lower_bound(key), upper_bound(key)};
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		const const_iterator position = lower_bound(key);
		if (position != end() && !Compare{}(key, position->first)) {
			return position;
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return static_cast<size_type>(upper_bound(key) - lower_bound(key));
	}

	constexpr iterator insert(const value_type & element) {
		if (real_size == N) {
			detail::precondition_violated("ctc::multimap: insertion into a full multimap");
		}
		const size_type index = detail::upper_bound_index<Compare>(content, real_size, element.first, detail::key_is_first{});
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = element;
		++real_size;
		return content + index;
	}

	// removal erases the whole equal range, like std::multimap
	template <typename K> constexpr size_type erase(const K & key) {
		const size_type from = detail::lower_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
		const size_type upto = detail::upper_bound_index<Compare>(content, real_size, key, detail::key_is_first{});
		const size_type removed = upto - from;
		if (removed == 0) {
			return 0;
		}
		for (size_type i{from}; i + removed < real_size; ++i) {
			content[i] = content[i + removed];
		}
		for (size_type i{real_size - removed}; i < real_size; ++i) {
			content[i] = value_type{};
		}
		real_size -= removed;
		return removed;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = value_type{};
		}
		real_size = 0;
	}

	template <size_t M> constexpr multimap<Key, Value, M, Compare> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::multimap: content does not fit the new capacity");
		}
		multimap<Key, Value, M, Compare> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise in sorted order, across capacities
CTC_EXPORT template <typename Key, typename Value, size_t A, size_t B, typename Compare> constexpr bool operator==(const map<Key, Value, A, Compare> & lhs, const map<Key, Value, B, Compare> & rhs) {
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
CTC_EXPORT template <typename Key, typename Value, size_t A, size_t B, typename Compare> constexpr bool operator==(const multimap<Key, Value, A, Compare> & lhs, const multimap<Key, Value, B, Compare> & rhs) {
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

#ifndef CTC__UNORDERED_SET__HPP
#define CTC__UNORDERED_SET__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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
#include <type_traits>
#endif

// Fixed-capacity constexpr unordered sets (std::unordered_set /
// std::unordered_multiset). Same design as unordered_map.hpp: a flat
// array scanned linearly with KeyEqual, iteration in insertion order
// (guaranteed), Hash accepted for interface compatibility and ignored.

namespace ctc {

CTC_EXPORT template <typename Key, size_t N, typename Hash = void, typename KeyEqual = equal_to> struct unordered_set {
	static_assert(std::is_default_constructible_v<Key>, "ctc::unordered_set<Key, N> requires a default-constructible Key");
	static_assert(std::is_empty_v<KeyEqual>, "ctc::unordered_set: KeyEqual must be stateless (it is not stored)");

	using key_type = Key;
	using value_type = Key;
	using size_type = size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using iterator = const Key *; // set elements are immutable, like std
	using const_iterator = const Key *;

	// every slot past size() is kept equal to Key{} by the mutators
	Key content[N ? N : 1]{};
	size_type real_size{0};

	constexpr unordered_set() noexcept = default;

	constexpr unordered_set(std::initializer_list<Key> init) {
		for (const Key & key : init) {
			insert(key);
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

	// iteration, in insertion order (guaranteed)
	constexpr const_iterator begin() const noexcept {
		return content;
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
	constexpr const Key * data() const noexcept {
		return content;
	}

	// lookup
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (KeyEqual{}(content[i], key)) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return find(key) != end() ? 1 : 0;
	}

	// insertion (appends; an existing key is left untouched)
	constexpr pair<const_iterator, bool> insert(const Key & key) {
		if (const const_iterator position = find(key); position != end()) {
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_set: insertion into a full set");
		}
		content[real_size] = key;
		return {content + real_size++, true};
	}

	// removal (later elements shift down, the vacated slot is reset)
	template <typename K> constexpr size_type erase(const K & key) {
		const const_iterator position = find(key);
		if (position == end()) {
			return 0;
		}
		const size_type index = static_cast<size_type>(position - content);
		for (size_type i{index}; i + 1 < real_size; ++i) {
			content[i] = content[i + 1];
		}
		content[--real_size] = Key{};
		return 1;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size = 0;
	}

	// a copy with a different capacity (checked to fit)
	template <size_t M> constexpr unordered_set<Key, M, Hash, KeyEqual> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::unordered_set: content does not fit the new capacity");
		}
		unordered_set<Key, M, Hash, KeyEqual> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// std::unordered_multiset: duplicates allowed and kept ADJACENT to
// their equals (equal_range works); groups keep first-insertion order.
CTC_EXPORT template <typename Key, size_t N, typename Hash = void, typename KeyEqual = equal_to> struct unordered_multiset {
	static_assert(std::is_default_constructible_v<Key>, "ctc::unordered_multiset<Key, N> requires a default-constructible Key");
	static_assert(std::is_empty_v<KeyEqual>, "ctc::unordered_multiset: KeyEqual must be stateless (it is not stored)");

	using key_type = Key;
	using value_type = Key;
	using size_type = size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using iterator = const Key *;
	using const_iterator = const Key *;

	Key content[N ? N : 1]{};
	size_type real_size{0};

	constexpr unordered_multiset() noexcept = default;

	constexpr unordered_multiset(std::initializer_list<Key> init) {
		for (const Key & key : init) {
			insert(key);
		}
	}

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

	constexpr const_iterator begin() const noexcept {
		return content;
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

	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (KeyEqual{}(content[i], key)) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		const const_iterator from = find(key);
		if (from == end()) {
			return {end(), end()};
		}
		const_iterator upto = from + 1;
		while (upto != end() && KeyEqual{}(*upto, key)) {
			++upto;
		}
		return {from, upto};
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		const auto [from, upto] = equal_range(key);
		return static_cast<size_type>(upto - from);
	}

	constexpr const_iterator insert(const Key & key) {
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_multiset: insertion into a full multiset");
		}
		size_type index = real_size;
		if (find(key) != end()) {
			index = static_cast<size_type>(equal_range(key).second - content);
		}
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = key;
		++real_size;
		return content + index;
	}

	// removal erases the whole equal range
	template <typename K> constexpr size_type erase(const K & key) {
		const auto [from, upto] = equal_range(key);
		const size_type removed = static_cast<size_type>(upto - from);
		if (removed == 0) {
			return 0;
		}
		const size_type index = static_cast<size_type>(from - content);
		for (size_type i{index}; i + removed < real_size; ++i) {
			content[i] = content[i + removed];
		}
		for (size_type i{real_size - removed}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size -= removed;
		return removed;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = Key{};
		}
		real_size = 0;
	}

	template <size_t M> constexpr unordered_multiset<Key, M, Hash, KeyEqual> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::unordered_multiset: content does not fit the new capacity");
		}
		unordered_multiset<Key, M, Hash, KeyEqual> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise in stored order, across capacities
CTC_EXPORT template <typename Key, size_t A, size_t B, typename Hash, typename KeyEqual> constexpr bool operator==(const unordered_set<Key, A, Hash, KeyEqual> & lhs, const unordered_set<Key, B, Hash, KeyEqual> & rhs) {
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
CTC_EXPORT template <typename Key, size_t A, size_t B, typename Hash, typename KeyEqual> constexpr bool operator==(const unordered_multiset<Key, A, Hash, KeyEqual> & lhs, const unordered_multiset<Key, B, Hash, KeyEqual> & rhs) {
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

#ifndef CTC__UNORDERED_MAP__HPP
#define CTC__UNORDERED_MAP__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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
#include <type_traits>
#endif

// Fixed-capacity constexpr unordered maps (std::unordered_map /
// std::unordered_multimap). At compile-time sizes a hash table buys
// nothing and its probe layout would poison NTTP identity, so the
// implementation is a flat array scanned linearly with KeyEqual - and
// as a documented GUARANTEE (stronger than std), iteration is in
// insertion order: an unordered_map models a JSON object. The Hash
// parameter is accepted for interface compatibility and ignored.
//
// Lookups are heterogeneous: any key type KeyEqual accepts works (a
// std::string_view against ctc::string keys, for instance).

namespace ctc {

CTC_EXPORT template <typename Key, typename Value, size_t N, typename Hash = void, typename KeyEqual = equal_to> struct unordered_map {
	static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>, "ctc::unordered_map<Key, Value, N> requires default-constructible Key and Value");
	static_assert(std::is_empty_v<KeyEqual>, "ctc::unordered_map: KeyEqual must be stateless (it is not stored)");

	using key_type = Key;
	using mapped_type = Value;
	using value_type = pair<Key, Value>;
	using size_type = size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using reference = value_type &;
	using const_reference = const value_type &;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	// every slot past size() is kept equal to value_type{} by the
	// mutators, so equal contents (same insertion order) mean
	// equivalent template arguments when a value is used as an NTTP
	value_type content[N ? N : 1]{};
	size_type real_size{0};

	constexpr unordered_map() noexcept = default;

	constexpr unordered_map(std::initializer_list<value_type> init) {
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

	// iteration, in insertion order (guaranteed)
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
			if (KeyEqual{}(content[i].first, key)) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (KeyEqual{}(content[i].first, key)) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		return find(key) != end() ? 1 : 0;
	}
	template <typename K> constexpr Value & at(const K & key) noexcept {
		const iterator position = find(key);
		if (position == end()) {
			detail::precondition_violated("ctc::unordered_map: at() with a key that is not in the map");
		}
		return position->second;
	}
	template <typename K> constexpr const Value & at(const K & key) const noexcept {
		const const_iterator position = find(key);
		if (position == end()) {
			detail::precondition_violated("ctc::unordered_map: at() with a key that is not in the map");
		}
		return position->second;
	}
	// like std::unordered_map, m[key] inserts a default-constructed
	// value for a missing key: m["answer"] = 42;
	template <typename K> constexpr Value & operator[](const K & key) {
		const iterator position = find(key);
		if (position != end()) {
			return position->second;
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_map: insertion into a full map");
		}
		content[real_size].first = Key{key};
		return content[real_size++].second;
	}

	// insertion (appends: insertion order is the iteration order)
	template <typename K> constexpr pair<iterator, bool> insert_or_assign(const K & key, const Value & value) {
		if (const iterator position = find(key); position != end()) {
			position->second = value;
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_map: insertion into a full map");
		}
		content[real_size] = value_type{Key{key}, value};
		return {content + real_size++, true};
	}
	// like std::unordered_map::insert, an existing key is left untouched
	constexpr pair<iterator, bool> insert(const value_type & element) {
		if (const iterator position = find(element.first); position != end()) {
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_map: insertion into a full map");
		}
		content[real_size] = element;
		return {content + real_size++, true};
	}
	template <typename K> constexpr pair<iterator, bool> try_emplace(const K & key, const Value & value) {
		if (const iterator position = find(key); position != end()) {
			return {position, false};
		}
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_map: insertion into a full map");
		}
		content[real_size] = value_type{Key{key}, value};
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
	template <size_t M> constexpr unordered_map<Key, Value, M, Hash, KeyEqual> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::unordered_map: content does not fit the new capacity");
		}
		unordered_map<Key, Value, M, Hash, KeyEqual> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// std::unordered_multimap: duplicate keys allowed. A new duplicate is
// inserted directly after the existing equal keys, so equal keys stay
// ADJACENT (equal_range works) while key groups keep first-insertion
// order.
CTC_EXPORT template <typename Key, typename Value, size_t N, typename Hash = void, typename KeyEqual = equal_to> struct unordered_multimap {
	static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>, "ctc::unordered_multimap<Key, Value, N> requires default-constructible Key and Value");
	static_assert(std::is_empty_v<KeyEqual>, "ctc::unordered_multimap: KeyEqual must be stateless (it is not stored)");

	using key_type = Key;
	using mapped_type = Value;
	using value_type = pair<Key, Value>;
	using size_type = size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	value_type content[N ? N : 1]{};
	size_type real_size{0};

	constexpr unordered_multimap() noexcept = default;

	constexpr unordered_multimap(std::initializer_list<value_type> init) {
		for (const value_type & element : init) {
			insert(element);
		}
	}

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

	template <typename K> constexpr const_iterator find(const K & key) const noexcept {
		for (size_type i{0}; i < real_size; ++i) {
			if (KeyEqual{}(content[i].first, key)) {
				return content + i;
			}
		}
		return end();
	}
	template <typename K> constexpr bool contains(const K & key) const noexcept {
		return find(key) != end();
	}
	template <typename K> constexpr size_type count(const K & key) const noexcept {
		const auto [from, upto] = equal_range(key);
		return static_cast<size_type>(upto - from);
	}
	// equal keys are adjacent (see insert), so the range is contiguous
	template <typename K> constexpr pair<const_iterator, const_iterator> equal_range(const K & key) const noexcept {
		const const_iterator from = find(key);
		if (from == end()) {
			return {end(), end()};
		}
		const_iterator upto = from + 1;
		while (upto != end() && KeyEqual{}(upto->first, key)) {
			++upto;
		}
		return {from, upto};
	}

	constexpr iterator insert(const value_type & element) {
		if (real_size == N) {
			detail::precondition_violated("ctc::unordered_multimap: insertion into a full multimap");
		}
		size_type index = real_size;
		if (const const_iterator existing = find(element.first); existing != end()) {
			index = static_cast<size_type>(equal_range(element.first).second - content);
		}
		for (size_type i{real_size}; i > index; --i) {
			content[i] = content[i - 1];
		}
		content[index] = element;
		++real_size;
		return content + index;
	}

	// removal erases the whole equal range, like std::unordered_multimap
	template <typename K> constexpr size_type erase(const K & key) {
		const auto [from, upto] = equal_range(key);
		const size_type removed = static_cast<size_type>(upto - from);
		if (removed == 0) {
			return 0;
		}
		const size_type index = static_cast<size_type>(from - content);
		for (size_type i{index}; i + removed < real_size; ++i) {
			content[i] = content[i + removed];
		}
		for (size_type i{real_size - removed}; i < real_size; ++i) {
			content[i] = value_type{};
		}
		real_size -= removed;
		return removed;
	}
	constexpr void clear() {
		for (size_type i{0}; i < real_size; ++i) {
			content[i] = value_type{};
		}
		real_size = 0;
	}

	template <size_t M> constexpr unordered_multimap<Key, Value, M, Hash, KeyEqual> with_capacity() const {
		if (real_size > M) {
			detail::precondition_violated("ctc::unordered_multimap: content does not fit the new capacity");
		}
		unordered_multimap<Key, Value, M, Hash, KeyEqual> result;
		for (size_type i{0}; i < real_size; ++i) {
			result.content[i] = content[i];
		}
		result.real_size = real_size;
		return result;
	}
};

// equality: element-wise in stored order, across capacities (the
// containers are insertion-ordered, and so is their equality)
CTC_EXPORT template <typename Key, typename Value, size_t A, size_t B, typename Hash, typename KeyEqual> constexpr bool operator==(const unordered_map<Key, Value, A, Hash, KeyEqual> & lhs, const unordered_map<Key, Value, B, Hash, KeyEqual> & rhs) {
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
CTC_EXPORT template <typename Key, typename Value, size_t A, size_t B, typename Hash, typename KeyEqual> constexpr bool operator==(const unordered_multimap<Key, Value, A, Hash, KeyEqual> & lhs, const unordered_multimap<Key, Value, B, Hash, KeyEqual> & rhs) {
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

#ifndef CTC__STACK__HPP
#define CTC__STACK__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// std::stack over a ctc container. The underlying container is a
// public member (std keeps it protected; public keeps the adaptor a
// structural type), named c like std's.

namespace ctc {

CTC_EXPORT template <typename T, size_t N, typename Container = vector<T, N>> struct stack {
	using container_type = Container;
	using value_type = T;
	using size_type = size_t;
	using reference = T &;
	using const_reference = const T &;

	Container c{};

	constexpr size_type size() const noexcept {
		return c.size();
	}
	static constexpr size_type capacity() noexcept {
		return Container::capacity();
	}
	constexpr bool empty() const noexcept {
		return c.empty();
	}
	constexpr bool full() const noexcept {
		return c.full();
	}

	constexpr void push(const T & value) {
		c.push_back(value);
	}
	template <typename... Args> constexpr reference emplace(Args &&... args) {
		return c.emplace_back(static_cast<Args &&>(args)...);
	}
	constexpr void pop() {
		if (c.empty()) {
			detail::precondition_violated("ctc::stack: pop on an empty stack");
		}
		c.pop_back();
	}
	constexpr reference top() {
		return c.back();
	}
	constexpr const_reference top() const {
		return c.back();
	}

	friend constexpr bool operator==(const stack &, const stack &) = default;
};

} // namespace ctc

#endif

#ifndef CTC__QUEUE__HPP
#define CTC__QUEUE__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

} // namespace ctc

#endif

#ifndef CTC__DEQUE__HPP
#define CTC__DEQUE__HPP

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

} // namespace detail

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

// transparent, stateless functors: the associative containers default
// to these and construct them on use instead of storing them (a
// stored comparator would be one more member in every NTTP)
CTC_EXPORT struct less {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a < b;
	}
};

CTC_EXPORT struct equal_to {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return a == b;
	}
};

namespace detail {

// The precondition trap. Calling a non-constexpr function is not a
// constant expression, so inside constant evaluation a violated
// precondition fails the compilation - with this function's name and
// its string argument visible in the constexpr backtrace. At runtime
// it aborts (the library is exception-free).
[[noreturn]] inline void precondition_violated(const char *) noexcept {
	std::abort();
}

// binary search over the sorted containers' storage. KeyOf projects an
// element to its key (identity for sets, .first for maps).
template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t lower_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (Compare{}(key_of(data[mid]), key)) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
}

template <typename Compare, typename Node, typename K, typename KeyOf>
constexpr size_t upper_bound_index(const Node * data, size_t size, const K & key, const KeyOf & key_of) noexcept {
	size_t low{0};
	size_t high{size};
	while (low < high) {
		const size_t mid = low + (high - low) / 2;
		if (!Compare{}(key, key_of(data[mid]))) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return low;
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

// A fixed-capacity constexpr deque. NOT a ring buffer: the front is
// always content[0], because a ring's head offset would make equal
// contents different template arguments (NTTP identity is layout
// identity). The price is O(size) front operations - irrelevant at
// compile-time sizes; the payoff is contiguous iteration and the same
// canonical-layout guarantee as every other ctc container. Publicly
// derives from ctc::vector (public bases keep a type structural), so
// the whole vector API is available.

namespace ctc {

CTC_EXPORT template <typename T, size_t N> struct deque : vector<T, N> {
	using vector<T, N>::vector;

	constexpr void push_front(const T & value) {
		if (this->size() == N) {
			detail::precondition_violated("ctc::deque: push_front on a full deque");
		}
		this->insert(this->begin(), value);
	}
	constexpr void pop_front() {
		if (this->empty()) {
			detail::precondition_violated("ctc::deque: pop_front on an empty deque");
		}
		this->erase(this->begin());
	}

	// a copy with a different capacity (shadows vector's, so shrunk<V>()
	// on a deque stays a deque)
	template <size_t M> constexpr deque<T, M> with_capacity() const {
		if (this->size() > M) {
			detail::precondition_violated("ctc::deque: content does not fit the new capacity");
		}
		deque<T, M> result;
		for (size_t i{0}; i < this->size(); ++i) {
			result.content[i] = this->content[i];
		}
		result.real_size = this->size();
		return result;
	}
};

} // namespace ctc

#endif

#ifndef CTC_IN_A_MODULE
#include <type_traits>
#endif

// std::queue and std::priority_queue over ctc containers. The
// underlying container is a public member (std keeps it protected;
// public keeps the adaptors structural), named c like std's.

namespace ctc {

CTC_EXPORT template <typename T, size_t N, typename Container = deque<T, N>> struct queue {
	using container_type = Container;
	using value_type = T;
	using size_type = size_t;
	using reference = T &;
	using const_reference = const T &;

	Container c{};

	constexpr size_type size() const noexcept {
		return c.size();
	}
	static constexpr size_type capacity() noexcept {
		return Container::capacity();
	}
	constexpr bool empty() const noexcept {
		return c.empty();
	}
	constexpr bool full() const noexcept {
		return c.full();
	}

	constexpr void push(const T & value) {
		c.push_back(value);
	}
	template <typename... Args> constexpr reference emplace(Args &&... args) {
		return c.emplace_back(static_cast<Args &&>(args)...);
	}
	constexpr void pop() {
		if (c.empty()) {
			detail::precondition_violated("ctc::queue: pop on an empty queue");
		}
		c.pop_front();
	}
	constexpr reference front() {
		return c.front();
	}
	constexpr const_reference front() const {
		return c.front();
	}
	constexpr reference back() {
		return c.back();
	}
	constexpr const_reference back() const {
		return c.back();
	}

	friend constexpr bool operator==(const queue &, const queue &) = default;
};

// std::priority_queue: a binary max-heap (with the default ctc::less,
// top() is the largest element, like std). The heap layout depends on
// the push/pop history, so unlike the canonical-layout containers,
// equal contents do NOT guarantee equivalent template arguments here -
// only operator== equality of the underlying storage.
CTC_EXPORT template <typename T, size_t N, typename Compare = less, typename Container = vector<T, N>> struct priority_queue {
	static_assert(std::is_empty_v<Compare>, "ctc::priority_queue: Compare must be stateless (it is not stored)");

	using container_type = Container;
	using value_type = T;
	using size_type = size_t;
	using const_reference = const T &;
	using value_compare = Compare;

	Container c{};

	constexpr size_type size() const noexcept {
		return c.size();
	}
	static constexpr size_type capacity() noexcept {
		return Container::capacity();
	}
	constexpr bool empty() const noexcept {
		return c.empty();
	}
	constexpr bool full() const noexcept {
		return c.full();
	}

	constexpr const_reference top() const {
		return c.front();
	}

	constexpr void push(const T & value) {
		c.push_back(value);
		// sift up
		size_type i = c.size() - 1;
		while (i > 0) {
			const size_type parent = (i - 1) / 2;
			if (Compare{}(c[parent], c[i])) {
				const T temporary = c[parent];
				c[parent] = c[i];
				c[i] = temporary;
				i = parent;
			} else {
				break;
			}
		}
	}
	constexpr void pop() {
		if (c.empty()) {
			detail::precondition_violated("ctc::priority_queue: pop on an empty priority_queue");
		}
		c[0] = c[c.size() - 1];
		c.pop_back();
		// sift down
		size_type i = 0;
		while (true) {
			const size_type left = 2 * i + 1;
			const size_type right = 2 * i + 2;
			size_type largest = i;
			if (left < c.size() && Compare{}(c[largest], c[left])) {
				largest = left;
			}
			if (right < c.size() && Compare{}(c[largest], c[right])) {
				largest = right;
			}
			if (largest == i) {
				break;
			}
			const T temporary = c[i];
			c[i] = c[largest];
			c[largest] = temporary;
			i = largest;
		}
	}
};

} // namespace ctc

#endif

// ctc: compile-time containers. Every C++20 standard container -
// array, vector, deque, forward_list, list, set/map/multiset/multimap,
// the four unordered_* containers, stack, queue, priority_queue - plus
// string and pair, as fixed-capacity constexpr types that stay
// STRUCTURAL - values built at compile time can be non-type template
// parameters and can persist to runtime in static storage:
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
