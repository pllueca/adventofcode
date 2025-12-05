// from https://adventofcode.com/2025/day/5

#include <filesystem>
#include <fstream>
#include <iostream>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

// std::string kInputFname = "2025/input5.txt";
string kInputFname = "2025/input5_full.txt";

typedef pair<long int, long int> Range;

struct RangesAndIngredients {
    vector<Range> ranges;
    vector<long int> ingredient_ids;
};

absl::Status ReadInput(const string input_filename,
                       RangesAndIngredients& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    bool reading_ranges = true;
    while (getline(file, line)) {
        if (line == "") {
            reading_ranges = false;
            continue;
        }

        if (reading_ranges) {
            // split by -
            vector<string> range_start_end = absl::StrSplit(line, "-");
            RETURNERRORIF(range_start_end.size() != 2,
                          absl::StatusCode::kUnknown,
                          "error reading range line: " + line);
            long int start, end;
            RETURNERRORIF(!absl::SimpleAtoi(range_start_end[0], &start),
                          absl::StatusCode::kInvalidArgument,
                          "error converting to int: " + range_start_end[0]);
            RETURNERRORIF(!absl::SimpleAtoi(range_start_end[1], &end),
                          absl::StatusCode::kInvalidArgument,
                          "error converting to int " + range_start_end[1]);
            input.ranges.push_back(make_pair(start, end));
        } else {
            long int ingredient_id;
            RETURNERRORIF(!absl::SimpleAtoi(line, &ingredient_id),
                          absl::StatusCode::kInvalidArgument,
                          "error converting to int " + line);
            input.ingredient_ids.push_back(ingredient_id);
        }
    }
    return absl::OkStatus();
}

void part1(const RangesAndIngredients& input) {
    int fresh_ingredients = 0;

    for (long int ingredient_id : input.ingredient_ids) {
        for (const Range& range : input.ranges) {
            if (ingredient_id >= range.first && ingredient_id <= range.second) {
                fresh_ingredients++;
                break;
            }
        }
    }
    cout << "part 1: " << fresh_ingredients << endl;
}

// looking at the ranges, how many ingredients are fresh
void part2(const RangesAndIngredients& input) {
    // sort ranges by min, max in case of tie
    auto ranges = input.ranges;
    sort(ranges.begin(), ranges.end());

    long int total_fresh = 0;
    long int current = 0;
    for (const Range& range : ranges) {
        // last range second is current
        // we are guaranteed that current > range.first
        long int start = range.first;
        long int end = range.second;
        if (current >= start) {
            // we already counted the ingrediesnt from start to current, advance
            // start
            start = current + 1;
        }
        if (start > end) {
            continue;
        }
        // cout << "adding from " << start << " to " << end << " (" << end -
        // start + 1 << ")" << endl;
        total_fresh += end - start + 1;
        current = end;
    }
    cout << "part 2: " << total_fresh << endl;
}

int main() {
    RangesAndIngredients input;
    auto res = ReadInput(kInputFname, input);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(input);
    part2(input);
    return 0;
}
