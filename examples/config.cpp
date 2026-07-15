#include <ctc.hpp>
#include <iostream>
#include <string_view>

// A tiny configuration table: assembled at compile time in a generous
// buffer, right-sized with shrunk<>(), then used at runtime out of
// static storage - no parsing, no allocation, no initialization order.

constexpr auto build_defaults() {
	ctc::map<ctc::string<24>, int, 32> m;
	m["max-connections"] = 64;
	m["timeout-ms"] = 2500;
	m["retries"] = 3;
	m["verbose"] = 0;
	return m;
}

constexpr auto defaults = ctc::shrunk<build_defaults()>();
static_assert(defaults.capacity() == defaults.size());
static_assert(defaults.at("retries") == 3);

int main() {
	std::cout << "defaults (" << defaults.size() << " entries):\n";
	for (const auto & [key, value] : defaults) {
		std::cout << "  " << key << " = " << value << '\n';
	}

	// runtime lookup, with a plain std::string_view key
	const std::string_view wanted{"timeout-ms"};
	if (const auto * found = defaults.find(wanted); found != defaults.end()) {
		std::cout << wanted << " -> " << found->second << '\n';
	}
}
