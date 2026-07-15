#include <ctc.hpp>
#include <type_traits>

// --- array: exactly N elements, aggregate, structural

constexpr ctc::array<int, 3> triple{1, 2, 3}; // brace elision
static_assert(triple.size() == 3 && !triple.empty());
static_assert(triple[0] == 1 && triple.front() == 1 && triple.back() == 3);
static_assert(std::is_same_v<decltype(ctc::array{1, 2, 3}), ctc::array<int, 3>>); // CTAD
static_assert(ctc::array{1, 2, 3} == triple);
static_assert(!(ctc::array{1, 2, 4} == triple));
static_assert([] {
	ctc::array<int, 4> a;
	a.fill(7);
	int sum{0};
	for (int value : a) {
		sum += value;
	}
	return sum;
}() == 28);
static_assert([] {
	ctc::array<int, 2> a{};
	a[1] = 5;
	return a[0] + a[1];
}() == 5);
static_assert(ctc::array<int, 0>{}.empty());

template <auto V> struct tag { };
static_assert(std::is_same_v<tag<ctc::array{1, 2}>, tag<ctc::array{1, 2}>>); // NTTP

// --- deque: vector plus front operations, canonical layout

constexpr auto dq = [] {
	ctc::deque<int, 8> d;
	d.push_back(2);
	d.push_front(1);
	d.push_back(3);
	d.push_back(9);
	d.pop_back();
	return d;
}();
static_assert(dq.size() == 3);
static_assert(dq[0] == 1 && dq[1] == 2 && dq[2] == 3);
static_assert(dq.front() == 1 && dq.back() == 3);
static_assert([] {
	auto d = dq;
	d.pop_front();
	return d.front() == 2 && d.size() == 2;
}());

// the whole vector API is inherited
static_assert([] {
	auto d = dq;
	d.insert(d.begin() + 1, 7);
	d.erase(d.begin());
	return d == ctc::deque<int, 8>{7, 2, 3};
}());

// canonical layout: front is content[0], so deques are NTTP-identity
// safe like vectors
constexpr auto front_built = [] {
	ctc::deque<int, 8> d;
	d.push_front(2);
	d.push_front(1);
	return d;
}();
constexpr auto back_built = [] {
	ctc::deque<int, 8> d;
	d.push_back(1);
	d.push_back(2);
	return d;
}();
static_assert(std::is_same_v<tag<front_built>, tag<back_built>>);

// shrunk of a deque stays a deque
constexpr auto shrunk_deque = ctc::shrunk<dq>();
static_assert(std::is_same_v<decltype(shrunk_deque), const ctc::deque<int, 3>>);
static_assert(shrunk_deque.capacity() == 3);

// --- forward_list

constexpr auto fwd = [] {
	ctc::forward_list<int, 8> l;
	l.push_front(3);
	l.push_front(1);
	auto it = l.begin();
	l.insert_after(it, 2); // 1 2 3
	return l;
}();
static_assert(fwd.size() == 3 && fwd.front() == 1);
static_assert([] {
	int walked[3]{};
	ctc::size_t i{0};
	for (int value : fwd) {
		walked[i++] = value;
	}
	return walked[0] == 1 && walked[1] == 2 && walked[2] == 3;
}());
static_assert([] {
	auto l = fwd;
	l.erase_after(l.begin()); // drops 2
	l.pop_front();            // drops 1
	return l.size() == 1 && l.front() == 3;
}());
static_assert(fwd == [] {
	ctc::forward_list<int, 4> same;
	same.push_front(3);
	same.push_front(2);
	same.push_front(1);
	return same;
}());

// a shrunk (compacted) forward_list is canonical, so lists become
// NTTP-identity safe after with_capacity/shrunk
constexpr auto fwd_canonical = ctc::shrunk<fwd>();
constexpr auto fwd_detoured = [] {
	auto l = fwd;
	l.push_front(0);
	l.pop_front();
	return l;
}();
constexpr auto fwd_canonical_again = ctc::shrunk<fwd_detoured>();
static_assert(std::is_same_v<tag<fwd_canonical>, tag<fwd_canonical_again>>);
static_assert(fwd_canonical.capacity() == 3);

// --- list (doubly linked)

constexpr auto lst = [] {
	ctc::list<int, 8> l;
	l.push_back(2);
	l.push_front(1);
	l.push_back(4);
	auto it = l.end();
	--it; // at 4
	l.insert(it, 3); // 1 2 3 4
	return l;
}();
static_assert(lst.size() == 4 && lst.front() == 1 && lst.back() == 4);
static_assert([] {
	int forward[4]{};
	ctc::size_t i{0};
	for (int value : lst) {
		forward[i++] = value;
	}
	int backward_first{0};
	auto it = lst.end();
	--it;
	backward_first = *it;
	return forward[2] == 3 && backward_first == 4;
}());
static_assert([] {
	auto l = lst;
	auto it = l.begin();
	++it;
	it = l.erase(it); // drops 2, lands on 3
	const bool landed = *it == 3;
	l.pop_back();
	l.pop_front();
	return landed && l.size() == 1 && l.front() == 3;
}());
static_assert(lst == [] {
	ctc::list<int, 16> same;
	for (int i = 1; i <= 4; ++i) {
		same.push_back(i);
	}
	return same;
}());
static_assert(ctc::shrunk<lst>().capacity() == 4);

// elements only reachable through iteration stay intact after erase
// in the middle (index links, not shifting)
static_assert([] {
	ctc::list<ctc::string<8>, 4> l;
	l.push_back(ctc::string<8>{"keep"});
	l.push_back(ctc::string<8>{"drop"});
	l.push_back(ctc::string<8>{"stay"});
	auto it = l.begin();
	++it;
	l.erase(it);
	return l.front() == "keep" && l.back() == "stay" && l.size() == 2;
}());

int main() { } // the suite is the static_asserts above
