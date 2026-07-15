#ifndef CTC__MAP__HPP
#define CTC__MAP__HPP

#include "utility.hpp"
#include "pair.hpp"

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
