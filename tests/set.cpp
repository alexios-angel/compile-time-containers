#include <ctc.hpp>
#include <type_traits>
#include <string_view>

// set: sorted, unique
constexpr auto built = [] {
	ctc::set<int, 8> s;
	s.insert(3);
	s.insert(1);
	s.insert(2);
	s.insert(2); // duplicate: untouched
	return s;
}();
static_assert(built.size() == 3);
static_assert(built.begin()[0] == 1 && built.begin()[2] == 3); // sorted
static_assert(built.contains(2) && !built.contains(9));
static_assert(*built.find(3) == 3 && built.find(9) == built.end());
static_assert(built.count(2) == 1 && built.count(9) == 0);
static_assert(*built.lower_bound(2) == 2 && *built.upper_bound(2) == 3);
static_assert([] {
	ctc::set<int, 2> s;
	const auto [inserted_it, inserted] = s.insert(5);
	return inserted && *inserted_it == 5;
}());

// initializer list, erase, clear
constexpr ctc::set<int, 8> from_init{5, 1, 5, 3};
static_assert(from_init.size() == 3 && from_init.begin()[0] == 1);
static_assert([] {
	auto s = from_init;
	return s.erase(3) == 1 && s.erase(9) == 0 && s.size() == 2 && !s.contains(3);
}());
static_assert([] {
	auto s = from_init;
	s.clear();
	return s.empty();
}());

// heterogeneous lookup with string keys
constexpr auto names = [] {
	ctc::set<ctc::string<8>, 4> s;
	s.insert(ctc::string<8>{"bob"});
	s.insert(ctc::string<8>{"alice"});
	return s;
}();
static_assert(names.contains(std::string_view{"alice"}));
static_assert(names.contains("bob") && !names.contains("carol"));
static_assert(*names.begin() == "alice"); // sorted

// canonical: same content, any insertion order, same NTTP
template <auto V> struct tag { };
constexpr auto one_order = [] {
	ctc::set<int, 8> s;
	s.insert(1);
	s.insert(2);
	return s;
}();
constexpr auto other_order = [] {
	ctc::set<int, 8> s;
	s.insert(2);
	s.insert(3);
	s.insert(1);
	s.erase(3);
	return s;
}();
static_assert(std::is_same_v<tag<one_order>, tag<other_order>>);
static_assert(one_order == other_order);

// multiset: duplicates, sorted, stable within equals; erase drops the
// whole equal range
constexpr auto multi = [] {
	ctc::multiset<int, 8> s;
	s.insert(2);
	s.insert(1);
	s.insert(2);
	return s;
}();
static_assert(multi.size() == 3 && multi.count(2) == 2);
static_assert(multi.begin()[0] == 1 && multi.begin()[1] == 2 && multi.begin()[2] == 2);
static_assert([] {
	const auto [from, upto] = multi.equal_range(2);
	return upto - from == 2;
}());
static_assert([] {
	auto s = multi;
	return s.erase(2) == 2 && s.size() == 1;
}());

// shrunk works on every sorted container
constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == 3 && shrunk == built);
static_assert(ctc::shrunk<multi>().capacity() == 3);

int main() { } // the suite is the static_asserts above
