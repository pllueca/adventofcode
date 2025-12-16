// from https://adventofcode.com/2025/day/12

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <unordered_map>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

string kInputFname = "2025/input12.txt";
// string kInputFname = "2025/input12_full.txt";

// only contains 1s and 0s
typedef matInt Shape;

struct Challenge {
    int width;
    int height;
    vecInt shape_amounts;
};

struct Input {
    unordered_map<int, Shape> shapes;
    vector<Challenge> challenges;

    string ToString() {
        return absl::StrCat("<Input ", "num_shapes: ", shapes.size(),
                            " num challenges: ", challenges.size(), ">");
    }

    // returns the shapes as a vector, each at the index of their id
    vector<Shape> ShapesVector() const {
        vector<Shape> r(shapes.size());
        for (int i = 0; i < shapes.size(); i++) {
            r[i] = shapes.at(i);
        }
        return r;
    }
};

absl::Status ReadInput(const string input_filename, Input& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    bool done_reading = false;
    std::regex challenge_pattern(R"delim(^(\d+)x(\d+): ((\d+ ?)+)$)delim");
    std::smatch matches;

    while (true) {
        // each shape is
        // int:\n + 3 lines contain 3 characters each, . or #
        done_reading = bool(getline(file, line));
        if (!done_reading) {
            break;
        }
        // read a shape
        // first number:
        if (line[line.size() - 1] == ':') {
            int shape_id;
            RETURNERRORIF(
                !absl::SimpleAtoi(line.substr(0, line.size() - 1), &shape_id),
                absl::StatusCode::kInvalidArgument,
                "error reading line: " + line);
            Shape s{matInt(3, vecInt(3, 0))};
            // read 3 lines of the shape
            for (int i = 0; i < 3; i++) {
                getline(file, line);
                // read the 3 first chars of the file only
                for (int j = 0; j < 3; j++) {
                    s[i][j] = line[j] == '#' ? 1 : 0;
                }
            }
            input.shapes.insert({shape_id, s});
            // read an empty line
            getline(file, line);

        } else if (std::regex_search(line, matches, challenge_pattern)) {
            int width, height;
            string width_str = matches[1];
            RETURNERRORIF(!absl::SimpleAtoi(width_str, &width),
                          absl::StatusCode::kInvalidArgument,
                          "error parsing as str: '" + width_str + "'");
            string height_str = matches[2];
            RETURNERRORIF(!absl::SimpleAtoi(height_str, &height),
                          absl::StatusCode::kInvalidArgument,
                          "error parsing as str: '" + height_str + "'");
            string nums = matches[3];
            vecInt shape_amounts;
            auto res = SeparateNumbersBySpace(nums, shape_amounts);
            RETURNERRORIF(!res.ok(), absl::StatusCode::kInvalidArgument,
                          "error");
            input.challenges.push_back({.width = width,
                                        .height = height,
                                        .shape_amounts = shape_amounts});

        } else {
            // shouldnt reach here
            return absl::Status(absl::StatusCode::kInvalidArgument,
                                "bad line: " + line);
        }
    }

    return absl::OkStatus();
}

// return true if we can fit the shape s in the map starting at i,j
// i and j are within bounds, and there is enough space
// (0 <= i < map.width - 2), (0 <= j < map.height - 2)
bool ShapeFits(const matInt& map, const Shape& shape, int i, int j) {
    // each shape is 3x3
    for (int si = 0; si < 3; si++) {
        for (int sj = 0; sj < 3; sj++) {
            if (shape[si][sj] == 1 && map[i + si][j + sj] != 0) {
                return false;
            }
        }
    }
    return true;
}

// update the map marking with 1 the positions that the shape s takes putting
// its left top corner in i,j we are guaranteed it fits
void PlaceShape(matInt& map, const Shape& shape, int i, int j, int color) {
    for (int si = 0; si < 3; si++) {
        for (int sj = 0; sj < 3; sj++) {
            if (shape[si][sj] == 1) {
                map[i + si][j + sj] = color;
            }
        }
    }
}

// rotate 90 deg clockwise
Shape Rotate90Deg(const Shape& s) {
    // 00 01 02
    // 10 11 12
    // 20 21 22
    // becomes
    // 20 10 00
    // 21 11 01
    // 22 12 02
    Shape s_rotated = Shape(3, vecInt(3, 0));
    s_rotated[0][0] = s[2][0];
    s_rotated[0][1] = s[1][0];
    s_rotated[0][2] = s[0][0];
    //
    s_rotated[1][0] = s[2][1];
    s_rotated[1][1] = s[1][1];
    s_rotated[1][2] = s[0][1];
    //
    s_rotated[2][0] = s[2][2];
    s_rotated[2][1] = s[1][2];
    s_rotated[2][2] = s[0][2];
    return s_rotated;
}

Shape FlipVertically(const Shape& s) {
    // 00 01 02
    // 10 11 12
    // 20 21 22
    // becomes
    // 20 21 22
    // 10 11 12
    // 00 01 02
    Shape s_flipped = Shape(3, vecInt(3, 0));

    // top row becomes botton row
    s_flipped[0][0] = s[2][0];
    s_flipped[0][1] = s[2][1];
    s_flipped[0][2] = s[2][2];

    // middle row keeps the same
    s_flipped[1][0] = s[1][0];
    s_flipped[1][1] = s[1][1];
    s_flipped[1][2] = s[1][2];

    // bottom row becomes top row
    s_flipped[2][0] = s[0][0];
    s_flipped[2][1] = s[0][1];
    s_flipped[2][2] = s[0][2];
    return s_flipped;
}

Shape FlipHorizontally(const Shape& s) {
    // 00 01 02
    // 10 11 12
    // 20 21 22
    // becomes
    // 02 01 00
    // 12 11 10
    // 22 21 20
    Shape s_flipped = Shape(3, vecInt(3, 0));

    // left col becomes right col
    s_flipped[0][0] = s[0][2];
    s_flipped[1][0] = s[1][2];
    s_flipped[2][0] = s[2][2];

    // middle col keeps the same
    s_flipped[0][1] = s[0][1];
    s_flipped[1][1] = s[1][1];
    s_flipped[2][1] = s[2][1];

    // right col becomes left col
    s_flipped[0][2] = s[0][0];
    s_flipped[1][2] = s[1][0];
    s_flipped[2][2] = s[2][0];
    return s_flipped;
}
// return a vector containing s, s rotated 90 degrees, s rotated 180 degrees,
// and s rotated 270 degrees
vector<Shape> ShapeRotations(const Shape& s) {
    Shape s90 = Rotate90Deg(s);
    Shape s180 = Rotate90Deg(s90);
    Shape s270 = Rotate90Deg(s180);
    vector<Shape> rotations{s, s90, s180, s270};
    return rotations;
}

vector<Shape> RotationsAndFlips(const Shape& s) {
    set<Shape> res;
    return {s};
    for (const Shape& rotated_s : ShapeRotations(s)) {
        res.insert(rotated_s);
        res.insert(FlipHorizontally(rotated_s));
        res.insert(FlipVertically(rotated_s));
    }
    return vector<Shape>(res.begin(), res.end());
}

bool CanFitRecursive(const matInt& map, const vecInt& remaining_shapes,
                     const vector<Shape>& shapes,
                     const vector<vector<Shape>>& transformed_shapes) {
    // used all the shapes and they fit!
    if (AllZeros(remaining_shapes)) {
        PRINT_MAT(map);
        return true;
    }

    for (int shape_idx = 0; shape_idx < shapes.size(); shape_idx++) {
        if (remaining_shapes[shape_idx] > 0) {
            // try to place this shape in each possible possition
            for (int i = 0; i < map.size() - 2; i++) {
                for (int j = 0; j < map[0].size() - 2; j++) {
                    // check if shape fits in map, with its top corner at i,j
                    for (const Shape& rotated_shape :
                         transformed_shapes[shape_idx]) {
                        if (ShapeFits(map, rotated_shape, i, j)) {
                            matInt map_updated = map;
                            PlaceShape(map_updated, rotated_shape, i, j,
                                       shape_idx);
                            vecInt remaining_shapes_updated = remaining_shapes;
                            remaining_shapes_updated[shape_idx]--;

                            // try to fit the rest of the shapes with the
                            // updated map
                            if (CanFitRecursive(map_updated,
                                                remaining_shapes_updated,
                                                shapes, transformed_shapes)) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool CanFit(const Challenge& challenge, const vector<Shape>& shapes) {
    // backtracking, similar to the 9 queens.
    // recurse passing the current matrix and the remainng shapes.
    // if no remaining shapes, then can fit
    // if remaining shapes, basically try each posibility
    // for each shape
    // for each empty position
    // if shape fits, put it and recurse
    matInt map = matInt(challenge.width, vecInt(challenge.height, 0));
    vector<vector<Shape>> transformed_shapes;
    for (int i = 0; i < shapes.size(); i++) {
        transformed_shapes.push_back(RotationsAndFlips(shapes[i]));
    }

    // precompute rotations and flips for each shape
    return CanFitRecursive(map, challenge.shape_amounts, shapes,
                           transformed_shapes);
}

void part1(const Input& input) {
    // for each challenge, check if can fit that many shapes
    int res = 0;
    auto shapes = input.ShapesVector();
    for (const Challenge& challenge : input.challenges) {
        if (CanFit(challenge, shapes)) {
            res++;
        }
    }
    cout << "part1: " << res << endl;
}

void part2() {}

int main() {
    Input input;
    auto res = ReadInput(kInputFname, input);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    cout << input.ToString() << endl;
    part1(input);
    part2();
    return 0;
}
