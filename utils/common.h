#ifndef ADVENTOFCODE_UTILS_H_
#define ADVENTOFCODE_UTILS_H_

#include <string>
#include <vector>

#include "absl/status/status.h"

#define PRINT_VECTOR(vec)                               \
    do {                                                \
        std::cout << #vec << ": [";                     \
        for (size_t __i = 0; __i < vec.size(); ++__i) { \
            std::cout << vec[__i];                      \
            if (__i < vec.size() - 1) {                 \
                std::cout << ", ";                      \
            }                                           \
        }                                               \
        std::cout << "]\n";                             \
    } while (0)

#define PRINT_SET(set)                   \
    do {                                 \
        std::cout << #set << ": {";      \
        size_t __i = 0;                  \
        for (const auto& __elem : set) { \
            std::cout << __elem;         \
            if (__i < set.size() - 1) {  \
                std::cout << ", ";       \
            }                            \
            ++__i;                       \
        }                                \
        std::cout << "}\n";              \
    } while (0)

#define PRINT_MAP(map)                            \
    do {                                          \
        std::cout << #map << ": {";               \
        size_t __i = 0;                           \
        for (const auto& [__key, __elem] : map) { \
            std::cout << __key << ": " << __elem; \
            if (__i < map.size() - 1) {           \
                std::cout << ", ";                \
            }                                     \
            ++__i;                                \
        }                                         \
        std::cout << "}\n";                       \
    } while (0)

#define PRINT_MAT(mat)                                         \
    do {                                                       \
        std::cout << #mat << ": [\n";                          \
        for (size_t __i = 0; __i < mat.size(); ++__i) {        \
            std::cout << "[";                                  \
            for (size_t __j = 0; __j < mat[0].size(); ++__j) { \
                std::cout << mat[__i][__j];                    \
                if (__j < mat[0].size() - 1) {                 \
                    std::cout << ", ";                         \
                }                                              \
            }                                                  \
            std::cout << "]";                                  \
            if (__i < mat.size() - 1) {                        \
                std::cout << ",\n";                            \
            }                                                  \
        }                                                      \
        std::cout << "]\n";                                    \
    } while (0)

#define RETURNERRORIF(condition, error, message) \
    if (condition) {                             \
        return absl::Status(error, message);     \
    }

#define STR2INT(str, int_var)                          \
    {                                                  \
        RETURNERRORIF(absl::SimpleAtoi(str, &int_var), \
                      "error parsing as str: " + str,  \
                      absl::Status::kInvalidArgument); \
    }

namespace aoc {
typedef std::vector<int> vecInt;
typedef std::vector<vecInt> matInt;

struct Point2d {
    double x, y;

    Point2d() : x(0), y(0) {}
    Point2d(double x, double y) : x(x), y(y) {}

    std::string ToString() {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
};

struct Point3d {
    double x, y, z;
    Point3d() : x(0), y(0), z(0) {}
    Point3d(double x, double y, double z) : x(x), y(y), z(z) {}

    std::string ToString() {
        return "(" + std::to_string(x) + "," + std::to_string(y) + "," +
               std::to_string(z) + ")";
    }
};

// Split a string by spaces, any number; i.e.:
// SplitBySpace("a b") -> ["a", "b"]
// SplitBySpace("a     b c") -> ["a", "b", "c"]
std::vector<std::string> SplitBySpace(const std::string& str);
std::vector<std::string> SplitByComma(const std::string& str);
absl::Status SeparateNumbersBySpace(const std::string& str, vecInt& res);

// return true if all the elements in the vector are 0.
bool AllZeros(const vecInt& v);

// euclidean distance
double Distance(const Point3d& p, const Point3d& q);

}  // namespace aoc

#endif  // ADVENTOFCODE_UTILS_H_
