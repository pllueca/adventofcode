#include "common.h"

#include <fstream>
#include <sstream>

namespace aoc
{

    std::vector<std::string> SplitBySpace(const std::string &str)
    {
        std::vector<std::string> tokens;
        std::istringstream ss(str);
        std::string word;

        while (ss >> word)
        {
            tokens.push_back(word);
        }
        return tokens;
    }

} // namespace aoc
