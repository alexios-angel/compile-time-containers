#include <ctc.hpp>
#include <type_traits>
#include <string_view>

// ctc::pair is structural and an aggregate
static_assert(ctc::pair{1, 2.0}.first == 1);
static_assert(ctc::make_pair(1, 'c').second == 'c');
static_assert(ctc::pair<int, int>{1, 2} == ctc::pair<int, int>{1, 2});
static_assert([] {
	auto [key, value] = ctc::pair{4, 2};
	return key * 10 + value;
}() == 42);

// building: operator[] inserts like std::map
using config = ctc::map<ctc::string<16>, int, 8>;
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

// heterogeneous lookup: any key type comparable with ==
static_assert(built.contains(std::string_view{"answer"}));
static_assert(built.at(ctc::string<8>{"answer"}) == 42);

// find
static_assert(built.find("answer")->second == 42);
static_assert(built.find("nope") == built.end());

// insertion order is preserved and iterable
static_assert(built.begin()[0].first == "answer" && built.begin()[1].first == "question");
static_assert([] {
	int sum{0};
	for (const auto & [key, value] : built) {
		sum += value;
	}
	return sum;
}() == 96);

// insert_or_assign vs insert
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

// initializer list: later duplicates overwrite, size stays right
constexpr ctc::map<int, int, 4> from_init{{1, 10}, {2, 20}, {1, 11}};
static_assert(from_init.size() == 2 && from_init.at(1) == 11);

// erase: order preserved, slot vacated
constexpr auto erased = [] {
	auto m = built;
	m["extra"] = 7;
	const ctc::map<ctc::string<16>, int, 8>::size_type removed = m.erase("question");
	return ctc::pair{m, removed};
}();
static_assert(erased.second == 1);
static_assert(erased.first.size() == 2);
static_assert(!erased.first.contains("question"));
static_assert(erased.first.begin()[0].first == "answer" && erased.first.begin()[1].first == "extra");
static_assert([] {
	auto m = built;
	return m.erase("nope");
}() == 0);

// equality is order-sensitive, across capacities
static_assert([] {
	ctc::map<int, int, 4> a;
	a[1] = 10;
	a[2] = 20;
	ctc::map<int, int, 9> same_order;
	same_order[1] = 10;
	same_order[2] = 20;
	ctc::map<int, int, 4> other_order;
	other_order[2] = 20;
	other_order[1] = 10;
	return a == same_order && !(a == other_order);
}());

// structural: values are NTTPs; erase resets the vacated slot so equal
// contents stay the SAME template argument
template <auto V> struct tag { };
constexpr auto direct = [] {
	ctc::map<int, int, 4> m;
	m[1] = 10;
	return m;
}();
constexpr auto detoured = [] {
	ctc::map<int, int, 4> m;
	m[1] = 10;
	m[2] = 20;
	m.erase(2);
	return m;
}();
static_assert(std::is_same_v<tag<direct>, tag<detoured>>);

// values can be containers; the whole thing stays constexpr
constexpr auto sections = [] {
	ctc::map<ctc::string<8>, ctc::vector<int, 4>, 4> m;
	m["evens"] = {2, 4, 6};
	m["odds"] = {1, 3, 5};
	m["evens"].push_back(8);
	return m;
}();
static_assert(sections.at("evens").size() == 4);
static_assert(sections.at("odds")[2] == 5);

// oversize, then right-size
constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == 2 && shrunk == built);
static_assert(std::is_same_v<decltype(shrunk), const ctc::map<ctc::string<16>, int, 2>>);

// clear
static_assert([] {
	auto m = built;
	m.clear();
	return m.empty() && m.capacity() == 8;
}());

int main() { } // the suite is the static_asserts above
