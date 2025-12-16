#include "common.h"

#include <fstream>
#include <sstream>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"

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

absl::Status SeparateNumbersBySpace(const std::string& str, vecInt& res) {
    int c;
    for (const std::string& part : SplitBySpace(str)) {
        RETURNERRORIF(!absl::SimpleAtoi(part, &c),
                      absl::StatusCode::kInvalidArgument,
                      "error reading ints from: " + str);
        res.push_back(c);
    }
    return absl::OkStatus();
}

bool AllZeros(const vecInt& v) {
    for (const int i : v) {
        if (i != 0) {
            return false;
        }
    }
    return true;
}

}  // namespace aoc
