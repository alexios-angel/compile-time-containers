#include <ctc.hpp>
#include <utility>

// the constexpr type-erased callable: calls, deep copies, moves and
// empties all fold in constant evaluation

constexpr int call_it(const ctc::cfunction<int(int)> & f, int x) { return f(x); }
static_assert(call_it(ctc::cfunction<int(int)>{[](int x) { return x * 2; }}, 21) == 42);

// captured state travels with the callable
static_assert([] {
	int base = 40;
	ctc::cfunction<int(int)> add{[base](int x) { return base + x; }};
	return add(2) == 42;
}());

// copy = deep clone; move steals; assignment releases the old callable
static_assert([] {
	ctc::cfunction<int()> a{[] { return 7; }};
	ctc::cfunction<int()> b = a;
	ctc::cfunction<int()> c = std::move(a);
	ctc::cfunction<int()> d;
	const bool empties = !d && static_cast<bool>(b) && static_cast<bool>(c);
	d = b;             // copy-assign over empty
	b = std::move(c);  // move-assign over engaged
	return empties && b() + d() == 14;
}());

// a different signature: reference parameters pass through unchanged
static_assert([] {
	ctc::cfunction<void(int &)> bump{[](int & x) { x += 5; }};
	int v = 37;
	bump(v);
	return v == 42;
}());
