#ifndef ADVENTOFCODE_2024_UTILS_H_
#define ADVENTOFCODE_2024_UTILS_H_

#include <string>
#include <vector>

namespace aoc2024
{

    // Split a string by spaces, any number; i.e.:
    // SplitBySpace("a b") -> ["a", "b"]
    // SplitBySpace("a     b c") -> ["a", "b", "c"]
    std::vector<std::string> SplitBySpace(const std::string &str);

} // namespace aoc2024

#endif // ADVENTOFCODE_2024_UTILS_H_
