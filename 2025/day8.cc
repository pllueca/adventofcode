// from https://adventofcode.com/2025/day/8

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

struct Point3d {
    double x, y, z;
    Point3d(int x, int y, int z) : x(x), y(y), z(z) {}

    string ToString() { return std::format("({},{},{})", x, y, z); }
};

double Distance(const Point3d& p, const Point3d& q) {
    // sqrt of sum of squared deltas
    return sqrt(pow(q.x - p.x, 2.0) + pow(q.y - p.y, 2.0) +
                pow(q.z - p.z, 2.0));
}

// std::string kInputFname = "2025/input8.txt";
// const int kMaxIterations = 10;

string kInputFname = "2025/input8_full.txt";
const int kMaxIterations = 1000;

absl::Status ReadInput(const string input_filename, vector<Point3d>& points) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        vector<string> coords = absl::StrSplit(line, ",");
        RETURNERRORIF(coords.size() != 3, absl::StatusCode::kUnknown,
                      "error reading input point: " + line);
        int x, y, z;
        RETURNERRORIF(!absl::SimpleAtoi(string(coords[0]), &x),
                      absl::StatusCode::kInvalidArgument,
                      "error reading x: " + coords[0]);
        RETURNERRORIF(!absl::SimpleAtoi(string(coords[1]), &y),
                      absl::StatusCode::kInvalidArgument,
                      "error reading x: " + coords[1]);
        RETURNERRORIF(!absl::SimpleAtoi(string(coords[2]), &z),
                      absl::StatusCode::kInvalidArgument,
                      "error reading x: " + coords[2]);
        points.push_back(Point3d(x, y, z));
    }
    return absl::OkStatus();
}

// set of the indices in an array of points
typedef set<int> Circuit;

vector<pair<int, int>> ComputeSortedDistances(const vector<Point3d>& points) {
    typedef tuple<double, int, int> DistanceAndPoint;
    vector<DistanceAndPoint> distances;
    // compute each point to point distance
    for (int p = 0; p < points.size(); p++) {
        for (int q = p + 1; q < points.size(); q++) {
            double distance = Distance(points[p], points[q]);
            distances.push_back({distance, p, q});
        }
    }

    // sort point (indices) by their distances
    sort(distances.begin(), distances.end());
    vector<pair<int, int>> points_sorted_by_distance;

    // keep only the point indices
    for (auto [_d, p, q] : distances) {
        points_sorted_by_distance.push_back({p, q});
    }
    return points_sorted_by_distance;
}
// connect the points p and q. merge the circuits where they belong
void ConnectPoints(int p, int q, map<int, int>& point_to_circuit) {
    // make each point in the same circuit as p point to the same circuit as q
    int source_circuit = point_to_circuit[p];
    int dest_circuit = point_to_circuit[q];
    for (auto& [point_idx, circuit_idx] : point_to_circuit) {
        if (circuit_idx == source_circuit) {
            point_to_circuit[point_idx] = dest_circuit;
        }
    }
}

// each step connect the 2 closest points. add the connected one to a
// circuit
void part1(const vector<Point3d>& points) {
    map<int, int> point_to_circuit;
    for (int i = 0; i < points.size(); ++i) {
        // initally each point is in the same circuit
        point_to_circuit.insert({i, i});
    }

    auto point_pairs_by_distance = ComputeSortedDistances(points);

    for (int i = 0; i < kMaxIterations; i++) {
        auto [p, q] = point_pairs_by_distance[i];
        ConnectPoints(p, q, point_to_circuit);
    }

    // find the 3 largest circuits, multiply its size
    vector<int> circuit_sizes(points.size());
    for (const auto& [point, circuit] : point_to_circuit) {
        ++circuit_sizes[circuit];
    }
    // sorted from large to small
    sort(circuit_sizes.begin(), circuit_sizes.end(), greater<int>());
    int res = circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
    cout << "part1 :" << res << endl;
}

int NumCircuits(const map<int, int> point_to_circuit) {
    vector<int> circuit_sizes(point_to_circuit.size());
    for (const auto& [point, circuit] : point_to_circuit) {
        ++circuit_sizes[circuit];
    }
    int num_non_empty = 0;
    for (int size : circuit_sizes) {
        if (size) {
            num_non_empty++;
        }
    }
    return num_non_empty;
}

void part2(const vector<Point3d>& points) {
    map<int, int> point_to_circuit;
    for (int i = 0; i < points.size(); ++i) {
        // initally each point is in the same circuit
        point_to_circuit.insert({i, i});
    }
    auto point_pairs_by_distance = ComputeSortedDistances(points);
    int num_circuits = points.size();
    long int res = -1;
    for (int i = 0; num_circuits > 1; i++) {
        auto [p_idx, q_idx] = point_pairs_by_distance[i];
        ConnectPoints(p_idx, q_idx, point_to_circuit);
        num_circuits = NumCircuits(point_to_circuit);
        if (num_circuits == 1) {
            Point3d p = points[p_idx];
            Point3d q = points[q_idx];
            // cout << "Last connection! between " << p.ToString() << " and "
            //      << q.ToString() << endl;
            res = p.x * q.x;
            break;
        }
    }
    cout << "part2 :" << res << endl;
}

int main() {
    vector<Point3d> points;
    auto res = ReadInput(kInputFname, points);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(points);
    part2(points);
    return 0;
}
