// from https://adventofcode.com/2025/day/4

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

std::string kInputFilename = "2025/input4.txt";
// const string kInputFilename = "2025/input4_full.txt";

// 8 possible directions
const int kMovesI[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int kMovesJ[] = {-1, 0, 1, 1, -1, -1, 0, 1};

absl::Status ReadInput(const string input_filename, matInt& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        vecInt line_as_ints;
        for (const char& c : line) {
            line_as_ints.push_back((c == '@') ? 1 : 0);
        }
        input.push_back(line_as_ints);
    }
    return absl::OkStatus();
}

// The forklifts can only access a roll of paper if there are fewer than four
// rolls of paper in the eight adjacent positions.
bool forkliftAccesible(const matInt& map, int i, int j) {
    int adjacent_paper_rolls = 0;
    for (int direction_idx = 0; direction_idx < 8; ++direction_idx) {
        int next_i = i + kMovesI[direction_idx];
        int next_j = j + kMovesJ[direction_idx];
        if (next_i >= 0 && next_i < map.size() && next_j >= 0 &&
            next_j < map[0].size() && map[next_i][next_j] == 1) {
            ++adjacent_paper_rolls;
            if (adjacent_paper_rolls >= 4) {
                return false;
            }
        }
    }
    return true;
}
/*
If you can figure out which rolls of paper the forklifts can access, they'll
spend less time looking and more time breaking down the wall to the cafeteria.
*/
void part1(const matInt& map) {
    const int n = map.size();
    const int m = map[0].size();
    int positions_accesible = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (map[i][j] == 1 && forkliftAccesible(map, i, j)) {
                ++positions_accesible;
            }
        }
    }
    cout << "part1: " << positions_accesible << endl;
}

// converts accessible toilet paper positions to 0s, returns the number removed
int removeAccesible(matInt& map) {
    vector<pair<int, int>> to_remove;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (map[i][j] == 1 && forkliftAccesible(map, i, j)) {
                to_remove.push_back(make_pair(i, j));
            }
        }
    }
    for (const auto& position : to_remove) {
        map[position.first][position.second] = 0;
    }
    return to_remove.size();
}
void part2(const matInt& map) {
    int total_removed = 0;
    matInt map_updated = map;
    int removed_this_step = removeAccesible(map_updated);
    while (removed_this_step) {
        total_removed += removed_this_step;
        removed_this_step = removeAccesible(map_updated);
    }

    cout << "part 2: " << total_removed << endl;
}

int main() {
    matInt input;
    auto res = ReadInput(kInputFilename, input);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(input);
    part2(input);
    return 0;
}
