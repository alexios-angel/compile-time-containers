#include <ctc.hpp>
#include <type_traits>
#include <string_view>

// ctc::pair is structural and an aggregate
static_assert(ctc::pair{1, 2.0}.first == 1);
static_assert(ctc::make_pair(1, 'c').second == 'c');
static_assert([] {
	auto [key, value] = ctc::pair{4, 2};
	return key * 10 + value;
}() == 42);

// building: operator[] inserts like std::map, at the sorted position
using config = ctc::map<ctc::string<16>, int, 8>;
constexpr auto built = [] {
	config m;
	m["question"] = 6 * 9;
	m["answer"] = 42;
	m["answer"] = 42; // an existing key is found, not duplicated
	return m;
}();
static_assert(built.size() == 2);
static_assert(built.contains("answer") && !built.contains("wat"));
static_assert(built.at("question") == 54);

// iteration is in sorted key order, regardless of insertion order
static_assert(built.begin()[0].first == "answer" && built.begin()[1].first == "question");

// heterogeneous lookup through the transparent ctc::less
static_assert(built.contains(std::string_view{"answer"}));
static_assert(built.at(ctc::string<8>{"answer"}) == 42);
static_assert(built.find("answer")->second == 42);
static_assert(built.find("nope") == built.end());
static_assert(built.lower_bound("b")->first == "question");
static_assert(built.count("answer") == 1 && built.count("nope") == 0);

// insert_or_assign / insert / try_emplace
static_assert([] {
	auto m = built;
	const auto [position, inserted] = m.insert_or_assign("question", 6 * 7);
	return !inserted && m.at("question") == 42 && position->first == "question";
}());
static_assert([] {
	auto m = built;
	const auto [position, inserted] = m.insert({ctc::string<16>{"of"}, 1});
	const auto [again, inserted_again] = m.insert({ctc::string<16>{"of"}, 2});
	return inserted && !inserted_again && m.at("of") == 1 && again == position;
}());
static_assert([] {
	auto m = built;
	const auto [position, inserted] = m.try_emplace("zzz", 7);
	return inserted && position->second == 7;
}());

// initializer list: later duplicates overwrite
constexpr ctc::map<int, int, 4> from_init{{2, 20}, {1, 10}, {1, 11}};
static_assert(from_init.size() == 2 && from_init.at(1) == 11 && from_init.begin()->first == 1);

// erase keeps things sorted and vacates the slot
static_assert([] {
	auto m = built;
	m["extra"] = 7;
	return m.erase("extra") == 1 && m.erase("nope") == 0 && m.size() == 2 && !m.contains("extra");
}());

// equality is content-based, across capacities and insertion orders
static_assert([] {
	ctc::map<int, int, 4> a;
	a[1] = 10;
	a[2] = 20;
	ctc::map<int, int, 9> b;
	b[2] = 20;
	b[1] = 10;
	return a == b;
}());

// structural, and CANONICAL: the same content is the same template
// argument even when built in a different insertion order
template <auto V> struct tag { };
constexpr auto one_order = [] {
	ctc::map<int, int, 4> m;
	m[1] = 10;
	m[2] = 20;
	return m;
}();
constexpr auto other_order = [] {
	ctc::map<int, int, 4> m;
	m[2] = 20;
	m[1] = 10;
	m[3] = 30;
	m.erase(3);
	return m;
}();
static_assert(std::is_same_v<tag<one_order>, tag<other_order>>);

// a custom stateless comparator flips the order
struct greater_than {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return b < a;
	}
};
static_assert([] {
	ctc::map<int, int, 4, greater_than> m;
	m[1] = 10;
	m[3] = 30;
	return m.begin()->first == 3;
}());

// multimap: duplicates allowed, equal keys adjacent and stable
constexpr auto scores = [] {
	ctc::multimap<ctc::string<8>, int, 8> m;
	m.insert({ctc::string<8>{"b"}, 1});
	m.insert({ctc::string<8>{"a"}, 2});
	m.insert({ctc::string<8>{"b"}, 3});
	return m;
}();
static_assert(scores.size() == 3 && scores.count("b") == 2);
static_assert(scores.begin()[0].first == "a");
static_assert(scores.begin()[1].second == 1 && scores.begin()[2].second == 3); // stable
static_assert([] {
	const auto [from, upto] = scores.equal_range("b");
	int sum{0};
	for (auto it = from; it != upto; ++it) {
		sum += it->second;
	}
	return sum;
}() == 4);
static_assert([] {
	auto m = scores;
	return m.erase("b") == 2 && m.size() == 1;
}());

// values can be containers; oversize-then-right-size still applies
constexpr auto sections = [] {
	ctc::map<ctc::string<8>, ctc::vector<int, 4>, 4> m;
	m["odds"] = {1, 3, 5};
	m["evens"] = {2, 4, 6};
	m["evens"].push_back(8);
	return m;
}();
static_assert(sections.at("evens").size() == 4 && sections.at("odds")[2] == 5);

constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == 2 && shrunk == built);
static_assert(std::is_same_v<decltype(shrunk), const ctc::map<ctc::string<16>, int, 2>>);

static_assert([] {
	auto m = built;
	m.clear();
	return m.empty() && m.capacity() == 8;
}());

int main() { } // the suite is the static_asserts above
