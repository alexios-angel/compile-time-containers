#include <ctc.hpp>
#include <type_traits>

// --- stack

constexpr auto stacked = [] {
	ctc::stack<int, 8> s;
	s.push(1);
	s.push(2);
	s.emplace(3);
	s.pop();
	return s;
}();
static_assert(stacked.size() == 2 && stacked.top() == 2);
static_assert(!stacked.empty() && !stacked.full() && stacked.capacity() == 8);
static_assert(stacked == [] {
	ctc::stack<int, 8> s;
	s.push(1);
	s.push(2);
	return s;
}());

// the underlying container is public and structural: NTTP works
template <auto V> struct tag { };
static_assert(std::is_same_v<tag<stacked>, tag<stacked>>);

// --- queue (FIFO, over the canonical deque)

constexpr auto queued = [] {
	ctc::queue<int, 8> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.pop();
	return q;
}();
static_assert(queued.size() == 2);
static_assert(queued.front() == 2 && queued.back() == 3);

// --- priority_queue (max-heap with the default ctc::less)

constexpr auto prioritized = [] {
	ctc::priority_queue<int, 8> q;
	q.push(3);
	q.push(1);
	q.push(4);
	q.push(1);
	q.push(5);
	return q;
}();
static_assert(prioritized.size() == 5 && prioritized.top() == 5);

// popping drains in descending order
static_assert([] {
	auto q = prioritized;
	int previous = q.top();
	while (!q.empty()) {
		if (q.top() > previous) {
			return false;
		}
		previous = q.top();
		q.pop();
	}
	return true;
}());

// a flipped comparator makes it a min-heap
struct greater_than {
	using is_transparent = void;
	template <typename A, typename B> constexpr bool operator()(const A & a, const B & b) const {
		return b < a;
	}
};
static_assert([] {
	ctc::priority_queue<int, 4, greater_than> q;
	q.push(3);
	q.push(1);
	q.push(2);
	return q.top() == 1;
}());

int main() { } // the suite is the static_asserts above
