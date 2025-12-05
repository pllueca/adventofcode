#ifndef ADVENTOFCODE_UTILS_H_
#define ADVENTOFCODE_UTILS_H_

#include <string>
#include <vector>

#define PRINT_VECTOR(vec)                         \
    std::cout << #vec << ": [";                   \
    for (size_t __i = 0; __i < vec.size(); ++__i) \
    {                                             \
        std::cout << vec[__i];                    \
        if (__i < vec.size() - 1)                 \
        {                                         \
            std::cout << ", ";                    \
        }                                         \
    }                                             \
    std::cout << "]\n";

#define PRINT_MAT(mat)                                   \
    std::cout << #mat << ": [\n";                        \
    for (size_t __i = 0; __i < mat.size(); ++__i)        \
    {                                                    \
        std::cout << "[";                                \
        for (size_t __j = 0; __j < mat[0].size(); ++__j) \
        {                                                \
            std::cout << mat[__i][__j];                  \
            if (__j < mat[0].size() - 1)                 \
            {                                            \
                std::cout << ", ";                       \
            }                                            \
        }                                                \
        std::cout << "]";                                \
        if (__i < mat.size() - 1)                        \
        {                                                \
            std::cout << ",\n";                          \
        }                                                \
    }                                                    \
    std::cout << "]\n";

#define RETURNERRORIF(condition, error, message) \
    if (condition)                               \
    {                                            \
        return absl::Status(                     \
            error,                               \
            message);                            \
    }

namespace aoc
{
    typedef std::vector<int> vecInt;
    typedef std::vector<vecInt> matInt;
    // Split a string by spaces, any number; i.e.:
    // SplitBySpace("a b") -> ["a", "b"]
    // SplitBySpace("a     b c") -> ["a", "b", "c"]
    std::vector<std::string> SplitBySpace(const std::string &str);

} // namespace aoc

#endif // ADVENTOFCODE_UTILS_H_
