#ifndef ADVENTOFCODE_UTILS_H_
#define ADVENTOFCODE_UTILS_H_

#include <string>
#include <vector>

#define PRINT_VECTOR(vec)                   \
    std::cout << #vec << ": [";             \
    for (size_t i = 0; i < vec.size(); ++i) \
    {                                       \
        std::cout << vec[i];                \
        if (i < vec.size() - 1)             \
        {                                   \
            std::cout << ", ";              \
        }                                   \
    }                                       \
    std::cout << "]\n";

namespace aoc
{
    typedef std::vector<int> vecInt;
    // Split a string by spaces, any number; i.e.:
    // SplitBySpace("a b") -> ["a", "b"]
    // SplitBySpace("a     b c") -> ["a", "b", "c"]
    std::vector<std::string> SplitBySpace(const std::string &str);

} // namespace aoc

#endif // ADVENTOFCODE_UTILS_H_
