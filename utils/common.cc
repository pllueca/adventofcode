#include "common.h"

#include <fstream>
#include <sstream>

namespace aoc {

std::vector<std::string> SplitBySpace(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream ss(str);
    std::string word;

    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}
std::vector<std::string> SplitByComma(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream ss(str);
    std::string word;

    while ((std::getline(ss, word, ','))) {
        tokens.push_back(word);
    }
    return tokens;
}

double Distance(const Point3d& p, const Point3d& q) {
    // sqrt of sum of squared deltas
    return sqrt(pow(q.x - p.x, 2.0) + pow(q.y - p.y, 2.0) +
                pow(q.z - p.z, 2.0));
}

}  // namespace aoc
