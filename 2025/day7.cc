// from https://adventofcode.com/2025/day/7

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

// std::string kInputFname = "2025/input7.txt";
string kInputFname = "2025/input7_full.txt";

struct PuzzleInput {
    vector<set<int>> splitters;
    int start_position;
    int lenght;
};

absl::Status ReadInput(const string input_filename, PuzzleInput& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    bool first_line = true;
    while (getline(file, line)) {
        // record the starter position
        if (first_line) {
            first_line = false;
            input.lenght = line.size();
            // record the position of S
            input.start_position = -1;
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == 'S') {
                    input.start_position = i;
                    break;
                }
            }
            RETURNERRORIF(input.start_position == -1,
                          absl::StatusCode::kInvalidArgument,
                          "Starting position not found in line: " + line);
        } else {
            // record the position of the splitters in this level
            set<int> splitters;
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == '^') {
                    splitters.insert(i);
                }
            }
            input.splitters.push_back(splitters);
        }
    }
    return absl::OkStatus();
}

void part1(const PuzzleInput& input) {
    // at each level, each active goes down.
    // if there is a spliter, add an active to each side
    int num_split = 0;
    set<int> current_active = {input.start_position};
    for (int level = 0; level < input.splitters.size(); level++) {
        string current_line(input.lenght, '.');
        set<int> new_active;
        const set<int>& level_splitters = input.splitters[level];
        for (int splitter_id : level_splitters) {
            current_line[splitter_id] = '^';
        }
        for (int active_position : current_active) {
            //
            if (level_splitters.contains(active_position)) {
                // add left n right
                num_split++;
                if (active_position > 0) {
                    new_active.insert(active_position - 1);
                }
                if (active_position < input.lenght - 1) {
                    new_active.insert(active_position + 1);
                }
            } else {
                // add just this one
                new_active.insert(active_position);
            }
        }
        for (int active_id : current_active) {
            current_line[active_id] = '|';
        }
        // PRINT_SET(current_active);
        // PRINT_SET(new_active);
        cout << current_line << endl;
        current_active = new_active;
    }
    cout << "part 1: " << num_split << endl;
}
void part2(const PuzzleInput& input) {}

int main() {
    PuzzleInput input;
    auto res = ReadInput(kInputFname, input);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(input);
    part2(input);
    return 0;
}
