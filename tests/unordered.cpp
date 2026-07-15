#include <ctc.hpp>
#include <type_traits>
#include <string_view>

// unordered_map: the insertion-ordered JSON-object workhorse
using config = ctc::unordered_map<ctc::string<16>, int, 8>;
constexpr auto built = [] {
	config m;
	m["answer"] = 42;
	m["question"] = 6 * 9;
	m["answer"] = 42; // an existing key is found, not duplicated
	return m;
}();
static_assert(built.size() == 2);
static_assert(built.contains("answer") && !built.contains("wat"));
static_assert(built.at("question") == 54);
static_assert(built.count("answer") == 1);

// iteration order is insertion order - GUARANTEED, unlike std
static_assert(built.begin()[0].first == "answer" && built.begin()[1].first == "question");

// heterogeneous lookup through the transparent ctc::equal_to
static_assert(built.contains(std::string_view{"answer"}));
static_assert(built.find("answer")->second == 42);
static_assert(built.find("nope") == built.end());

// insert_or_assign / insert / try_emplace / erase (order-preserving)
static_assert([] {
	auto m = built;
	const auto [position, inserted] = m.insert_or_assign("question", 6 * 7);
	const auto [ignored, emplaced] = m.try_emplace("of", 1);
	m["extra"] = 9;
	const bool order_kept = m.erase("of") == 1 && m.begin()[2].first == "extra";
	return !inserted && emplaced && m.at("question") == 42 && order_kept;
}());

// structural: equal content built the same way is the same NTTP
// (the vacated-slot reset at work)
template <auto V> struct tag { };
constexpr auto direct = [] {
	ctc::unordered_map<int, int, 4> m;
	m[1] = 10;
	return m;
}();
constexpr auto detoured = [] {
	ctc::unordered_map<int, int, 4> m;
	m[1] = 10;
	m[2] = 20;
	m.erase(2);
	return m;
}();
static_assert(std::is_same_v<tag<direct>, tag<detoured>>);

constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == 2 && shrunk == built);

// unordered_set
constexpr auto seen = [] {
	ctc::unordered_set<ctc::string<8>, 4> s;
	s.insert(ctc::string<8>{"bob"});
	s.insert(ctc::string<8>{"alice"});
	s.insert(ctc::string<8>{"bob"});
	return s;
}();
static_assert(seen.size() == 2);
static_assert(*seen.begin() == "bob"); // insertion order
static_assert(seen.contains(std::string_view{"alice"}) && !seen.contains("carol"));
static_assert([] {
	auto s = seen;
	return s.erase("bob") == 1 && s.size() == 1 && *s.begin() == "alice";
}());

// unordered_multiset: duplicates kept adjacent to their group
constexpr auto multi = [] {
	ctc::unordered_multiset<int, 8> s;
	s.insert(7);
	s.insert(1);
	s.insert(7);
	return s;
}();
static_assert(multi.size() == 3 && multi.count(7) == 2);
static_assert(multi.begin()[0] == 7 && multi.begin()[1] == 7 && multi.begin()[2] == 1);
static_assert([] {
	const auto [from, upto] = multi.equal_range(7);
	return upto - from == 2;
}());
static_assert([] {
	auto s = multi;
	return s.erase(7) == 2 && s.size() == 1 && *s.begin() == 1;
}());

// unordered_multimap: same grouping, key groups in first-insertion order
constexpr auto tags = [] {
	ctc::unordered_multimap<ctc::string<8>, int, 8> m;
	m.insert({ctc::string<8>{"b"}, 1});
	m.insert({ctc::string<8>{"a"}, 2});
	m.insert({ctc::string<8>{"b"}, 3});
	return m;
}();
static_assert(tags.size() == 3 && tags.count("b") == 2);
static_assert(tags.begin()[0].second == 1 && tags.begin()[1].second == 3); // "b" group, stable
static_assert(tags.begin()[2].first == "a");
static_assert([] {
	int sum{0};
	const auto [from, upto] = tags.equal_range("b");
	for (auto it = from; it != upto; ++it) {
		sum += it->second;
	}
	return sum;
}() == 4);
static_assert([] {
	auto m = tags;
	return m.erase("b") == 2 && m.size() == 1 && m.begin()->first == "a";
}());

int main() { } // the suite is the static_asserts above
