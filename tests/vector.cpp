#include <ctc.hpp>
#include <type_traits>

// construction
static_assert(ctc::vector<int, 4>{}.empty());
static_assert(ctc::vector<int, 4>{}.capacity() == 4);
static_assert(ctc::vector<int, 4>{1, 2, 3}.size() == 3);
static_assert(ctc::vector<int, 4>{2, 7}.front() == 2);
static_assert(ctc::vector<int, 8>(3, 42)[2] == 42);
static_assert(ctc::vector<int, 2>{ctc::vector<int, 8>{1, 2}} == ctc::vector<int, 2>{1, 2}); // capacity conversion

// mutation
constexpr auto built = [] {
	ctc::vector<int, 8> v;
	for (int i = 1; i <= 4; ++i) {
		v.push_back(i * i);
	}
	v.emplace_back(25);
	return v;
}();
static_assert(built.size() == 5);
static_assert(built[0] == 1 && built[4] == 25 && built.back() == 25);
static_assert(!built.full() && !built.empty());

constexpr auto edited = [] {
	auto v = built;             // 1 4 9 16 25
	v.pop_back();               // 1 4 9 16
	v.erase(v.begin() + 1);     // 1 9 16
	v.insert(v.begin() + 1, 2); // 1 2 9 16
	v.resize(6, 7);             // 1 2 9 16 7 7
	return v;
}();
static_assert(edited == ctc::vector<int, 8>{1, 2, 9, 16, 7, 7});
static_assert(edited.size() == 6);

constexpr auto shrunk_by_resize = [] {
	auto v = edited;
	v.resize(2);
	return v;
}();
static_assert(shrunk_by_resize == ctc::vector<int, 8>{1, 2});

// erase returns the iterator past the removed element
static_assert([] {
	ctc::vector<int, 4> v{10, 20, 30};
	return *v.erase(v.begin());
}() == 20);

// iteration
static_assert([] {
	int sum{0};
	for (int value : built) {
		sum += value;
	}
	return sum;
}() == 55);

// equality is element-wise, across capacities
static_assert(ctc::vector<int, 4>{1, 2} == ctc::vector<int, 9>{1, 2});
static_assert(ctc::vector<int, 4>{1, 2} != ctc::vector<int, 4>{1, 3});
static_assert(ctc::vector<int, 4>{1, 2} != ctc::vector<int, 4>{1, 2, 3});

// element types: aggregates, pairs, strings, nested vectors
struct point {
	int x{};
	int y{};
	friend constexpr bool operator==(const point &, const point &) = default;
};
static_assert([] {
	ctc::vector<point, 4> v;
	v.push_back({1, 2});
	v.emplace_back(3, 4);
	return v[0].x + v[1].y;
}() == 5);
static_assert([] {
	ctc::vector<ctc::string<8>, 4> v;
	v.emplace_back("meaning");
	v.push_back(ctc::string<8>{"of"});
	return v[0] == "meaning" && v[1] == "of";
}());
static_assert([] {
	ctc::vector<ctc::vector<int, 3>, 3> grid;
	grid.push_back({1, 2, 3});
	grid.push_back({4, 5, 6});
	return grid[1][2];
}() == 6);
static_assert([] {
	ctc::vector<ctc::pair<int, int>, 3> v;
	v.emplace_back(1, 2);
	auto [a, b] = v[0];
	return a + b;
}() == 3);

// structural: values are NTTPs; equal content built differently is the
// SAME template argument (vacated slots are reset to T{})
template <auto V> struct tag { };
constexpr auto direct = ctc::vector<int, 8>{1, 2};
constexpr auto detoured = [] {
	ctc::vector<int, 8> v{1, 2, 3};
	v.pop_back();
	return v;
}();
static_assert(std::is_same_v<tag<direct>, tag<detoured>>);
constexpr auto detoured_by_erase = [] {
	ctc::vector<int, 8> v{1, 7, 2};
	v.erase(v.begin() + 1);
	return v;
}();
static_assert(std::is_same_v<tag<direct>, tag<detoured_by_erase>>);

// oversize, then right-size
constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == shrunk.size() && shrunk == built);
static_assert(std::is_same_v<decltype(shrunk), const ctc::vector<int, 5>>);

// a zero-capacity vector is a valid (empty) container
static_assert(ctc::vector<int, 0>{}.empty());
static_assert(ctc::vector<int, 0>{}.capacity() == 0);
static_assert(ctc::shrunk<ctc::vector<int, 4>{}>().capacity() == 0);

int main() { } // the suite is the static_asserts above
