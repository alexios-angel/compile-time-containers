#include <ctc.hpp>
#include <type_traits>
#include <string_view>

// construction
static_assert(ctc::string<8>{}.empty());
static_assert(ctc::string<8>{}.capacity() == 8);
static_assert(ctc::basic_string{"hello"}.size() == 5);
static_assert(ctc::basic_string{"hello"}.capacity() == 5);
static_assert(ctc::make_string("hello") == "hello");
static_assert(std::is_same_v<decltype(ctc::basic_string{"hello"}), ctc::string<5>>);
static_assert(ctc::string<8>{"hi"}.size() == 2);
static_assert(ctc::string<8>{std::string_view{"hi"}} == "hi");
static_assert(ctc::string<8>{3, 'x'} == "xxx");
static_assert(ctc::string<4>{ctc::string<8>{"hi"}} == "hi"); // capacity conversion

// other character types
static_assert(ctc::basic_string{U"chars"}.size() == 5);
static_assert(ctc::basic_string{u8"chars"} == u8"chars");
static_assert(ctc::basic_string{L"chars"} == L"chars");
static_assert(ctc::basic_string{u"chars"} == u"chars");

// c_str: the terminator is always there, not counted
static_assert(ctc::basic_string{"hi"}.c_str()[2] == '\0');
static_assert(std::string_view{ctc::basic_string{"hi"}.c_str()} == "hi");

// mutation
constexpr auto built = [] {
	ctc::string<16> s;
	s.append("Hello");
	s += ',';
	s += " World";
	s.push_back('!');
	return s;
}();
static_assert(built == "Hello, World!");
static_assert(built.size() == 13);
static_assert(!built.full());

constexpr auto resized = [] {
	ctc::string<8> s{"abc"};
	s.resize(5, 'x');
	s.pop_back();
	return s;
}();
static_assert(resized == "abcx");

constexpr auto cleared = [] {
	ctc::string<8> s{"abc"};
	s.clear();
	return s;
}();
static_assert(cleared.empty() && cleared == "");

// element access and iteration
static_assert(built[0] == 'H' && built.front() == 'H' && built.back() == '!');
static_assert([] {
	size_t count{0};
	for (char c : built) {
		if (c == 'l') {
			++count;
		}
	}
	return count;
}() == 3);
static_assert([] {
	ctc::string<4> s{"ab"};
	s[0] = 'x';
	return s;
}() == "xb");

// search
static_assert(built.find("World") == 7);
static_assert(built.find('!') == 12);
static_assert(built.find("xyz") == built.npos);
static_assert(built.rfind('l') == 10);
static_assert(built.contains("lo, W") && !built.contains("wat"));
static_assert(built.starts_with("Hello") && built.starts_with('H'));
static_assert(built.ends_with("World!") && built.ends_with('!'));
static_assert(built.compare("Hello") > 0);

// substr: compile-time capacity vs view
constexpr auto world = built.substr<7, 5>();
static_assert(world == "World" && world.capacity() == 5);
static_assert(built.substr(7, 5) == std::string_view{"World"});
static_assert(built.substr(7) == std::string_view{"World!"});

// comparison, in every direction C++20 synthesizes
static_assert(ctc::basic_string{"same"} == ctc::string<9>{"same"});
static_assert(ctc::basic_string{"same"} != ctc::basic_string{"other"});
static_assert(ctc::basic_string{"same"} == "same");
static_assert("same" == ctc::basic_string{"same"});
static_assert(ctc::basic_string{"apple"} < ctc::basic_string{"banana"});
static_assert(ctc::basic_string{"apple"} < "banana");
static_assert("apple" <= ctc::basic_string{"apple"});
static_assert(ctc::basic_string{"b"} > std::string_view{"a"});

// concatenation
static_assert(ctc::basic_string{"foo"} + ctc::basic_string{"bar"} == "foobar");
static_assert(ctc::basic_string{"foo"} + "bar" == "foobar");
static_assert("foo" + ctc::basic_string{"bar"} == "foobar");
static_assert((ctc::basic_string{"a"} + "b").capacity() == 2);

// conversion to std::string_view
static_assert(std::is_convertible_v<ctc::string<4>, std::string_view>);
static_assert(built.view().substr(0, 5) == "Hello");

// structural: values are NTTPs; equal content built differently is the
// SAME template argument (vacated units are reset to zero)
template <auto V> struct tag { };
constexpr auto direct = ctc::string<8>{"ab"};
constexpr auto detoured = [] {
	ctc::string<8> s{"abc"};
	s.pop_back();
	return s;
}();
static_assert(std::is_same_v<tag<direct>, tag<detoured>>);

// oversize, then right-size
constexpr auto shrunk = ctc::shrunk<built>();
static_assert(shrunk.capacity() == 13 && shrunk.capacity() == shrunk.size());
static_assert(shrunk == built);
static_assert(std::is_same_v<decltype(shrunk), const ctc::string<13>>);

// and the shrunk value persists to runtime: a static of minimal size
constexpr auto & runtime_visible = shrunk;
static_assert(sizeof(runtime_visible) <= sizeof(char) * 14 + sizeof(size_t) + alignof(size_t));

int main() { } // the suite is the static_asserts above
