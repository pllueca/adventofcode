// from https://adventofcode.com/2025/day/6

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

std::string kInputFname = "2025/input6.txt";
// string kInputFname = "2025/input6_full.txt";

struct Problems {
    matInt numbers;
    vector<vector<string>> numbers_as_strings;
    vector<char> ops;  // either + or *
};

absl::Status ReadInput(const string input_filename, Problems& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        // read lines of numbers until last line, which contains +/*
        if (line[0] == '+' || line[0] == '*') {
            for (string& op : SplitBySpace(line)) {
                RETURNERRORIF(!(op == "+" | op == "*"),
                              absl::StatusCode::kInvalidArgument,
                              "error reading ops line: " + line);
                input.ops.push_back(op.c_str()[0]);
            }
        } else {
            // read a line of numbers
            vecInt current_line;
            vector<string> current_numbers_as_str;
            for (const string& number : SplitBySpace(line)) {
                int num;
                RETURNERRORIF(!absl::SimpleAtoi(number, &num),
                              absl::StatusCode::kInvalidArgument,
                              "error reading " + number + " as int");
                current_line.push_back(num);
                current_numbers_as_str.push_back(number);
            }
            input.numbers.push_back(current_line);
            input.numbers_as_strings.push_back(current_numbers_as_str);
        }
    }
    return absl::OkStatus();
}

void part1(const Problems& problems) {
    /*
    Each problem's numbers are arranged vertically; at the bottom of the problem
    is the symbol for the operation that needs to be performed. Problems are
    separated by a full column of only spaces. The left/right alignment of
    numbers within each problem can be ignored.
    */
    long int total = 0;
    for (int i = 0; i < problems.numbers[0].size(); ++i) {
        // operate each column
        long int t = problems.ops[i] == '+' ? 0 : 1;  // 0 if sum, 1 if multiply
        for (int j = 0; j < problems.numbers.size(); ++j) {
            if (problems.ops[i] == '+') {
                t += problems.numbers[j][i];
            } else {
                t *= problems.numbers[j][i];
            }
        }
        total += t;
    }
    cout << "part1: " << total << endl;
}

/*
Cephalopod math is written right-to-left in columns. Each number is given in its
own column, with the most significant digit at the top and the least significant
digit at the bottom. (Problems are still separated with a column consisting only
of spaces, and the symbol at the bottom of the problem is still the operator to
use.) Here's the example worksheet again:

123 328  51 64
 45 64  387 23
  6 98  215 314
*   +   *   +

->> fill with 0s, but not the gaps between them

123 328 X51 64X
X45 64X 387 23X
XX6 98X 215 314
*   +   *   +

->>
4 431 623 +
175 581 32 *
8 248 369 +
356 24 1 *

64X
23X
314
->
xx4
431
623

*/
void part2(const Problems& problems) {
    long int total = 0;

    // each line must have lenght %max num digits + 1

    for (int i = 0; i < problems.numbers[0].size(); ++i) {
        vecInt current_nums;
        int max_lenght = 1;
        for (int j = 0; j < problems.numbers.size(); ++j) {
            current_nums.push_back(problems.numbers[j][i]);
            if (to_string(problems.numbers[j][i]).size() > max_lenght) {
                max_lenght = to_string(problems.numbers[j][i]).size();
            }
        }

        vector<string> padded_nums;
        for (int number : current_nums) {
            std::stringstream ss;
            ss << std::setw(max_lenght) << std::setfill('0') << number;
            std::string num_as_str = ss.str();
            padded_nums.push_back(num_as_str);
        }
        cout << "current op: " << problems.ops[i] << endl;
        PRINT_VECTOR(padded_nums);
        // 123
        // 045
        // 006
        // -> 356, 24, 1

        vecInt vertical_nums;
        for (int column = 0; column < max_lenght; ++column) {
            string current_vertical_num_as_str = "";
            for (int row = 0; row < current_nums.size(); ++row) {
                if (padded_nums[row][column] != '0') {
                    current_vertical_num_as_str += padded_nums[row][column];
                }
            }
            int current_vertical_num;
            if (!absl::SimpleAtoi(current_vertical_num_as_str,
                                  &current_vertical_num)) {
                cout << "error converting " << current_vertical_num_as_str
                     << endl;
                return;
            }
            vertical_nums.push_back(current_vertical_num);
        }
        PRINT_VECTOR(vertical_nums);

        long int t = problems.ops[i] == '+' ? 0 : 1;  // 0 if sum, 1 if multiply
        for (int num : vertical_nums) {
            if (problems.ops[i] == '+') {
                t += num;
            } else {
                t *= num;
            }
        }

        total += t;
    }
    cout << "part2: " << total << endl;
}

int main() {
    Problems problems;
    auto res = ReadInput(kInputFname, problems);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(problems);
    part2(problems);
    return 0;
}
