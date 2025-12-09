// from https://adventofcode.com/2025/day/9

#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

std::string kInputFname = "2025/input9.txt";
// string kInputFname = "2025/input9_full.txt";

absl::Status ReadInput(const string input_filename, vector<Point2d>& points) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        vector<string> coords = absl::StrSplit(line, ",");
        RETURNERRORIF(coords.size() != 2, absl::StatusCode::kUnknown,
                      "error reading input point: " + line);
        long int x, y;
        RETURNERRORIF(!absl::SimpleAtoi(string(coords[0]), &x),
                      absl::StatusCode::kInvalidArgument,
                      "error reading x: " + coords[0]);
        RETURNERRORIF(!absl::SimpleAtoi(string(coords[1]), &y),
                      absl::StatusCode::kInvalidArgument,
                      "error reading x: " + coords[1]);
        points.push_back(Point2d(x, y));
    }
    return absl::OkStatus();
}

long double AreaRectangleBetweenTwoPoints(const Point2d& p, const Point2d& q) {
    return (abs(p.x - q.x) + 1) * (abs(p.y - q.y) + 1);
}

void part1(const vector<Point2d>& points) {
    long double max_area = 0;
    vector<tuple<double, int, int>> points_and_areas;
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            long double area =
                AreaRectangleBetweenTwoPoints(points[i], points[j]);
            points_and_areas.push_back({area, i, j});
            max_area = max(max_area, area);
        }
    }

    // sort by area, larger first
    sort(points_and_areas.begin(), points_and_areas.end());
    reverse(points_and_areas.begin(), points_and_areas.end());
    Point2d p, q;
    p = points[get<1>(points_and_areas[0])];
    q = points[get<2>(points_and_areas[0])];
    long int max_area_as_int = max_area;
    cout << "max area " << get<0>(points_and_areas[0]) << " p: " << p.ToString()
         << " q: " << q.ToString() << endl;
    cout << "part 1:" << max_area_as_int << endl;
}

struct Edge {
    Point2d src;
    Point2d dst;

    bool Intersect(const Edge& other) {
        // todo
        return false;
    }
};

struct Polygon {
    vector<Edge> edges;

    bool ContainsPoint(const Point2d& p) {
        // ray tracing:

        // trace a line from p to {p.x, inf}
        Edge ray{.src = p, .dst = Point2d(p.x, numeric_limits<double>::max())};
        // count how many edges of the polygon intersects
        int edge_intersection_count = 0;
        for (const Edge& polygon_edge : edges) {
            if (ray.Intersect(polygon_edge)) {
                edge_intersection_count++;
            }
        }
        // if even number, it is outside. if odd, its inside
        return (edge_intersection_count % 2) != 0;
    }
    bool ContainsRectangle(const Point2d& top_left,
                           const Point2d& bottom_right) {
        // tl x1,y1
        // br x2, y2
        // tr x1, y2
        // bl x2, y1
        Point2d top_right(top_left.x, bottom_right.y);
        Point2d bottom_left(bottom_right.x, top_left.y);
        // check if each of the corners of the rectangle is inside the polygon
        for (const Point2d& p :
             {top_left, bottom_right, top_right, bottom_left}) {
            if (!ContainsPoint(p)) {
                return false;
            }
        }
        return true;
    }
};

void part2(const vector<Point2d>& points) {
    // considering the polygon formed by the points, find the largest rectangle
    // whose opoosing corners belong to the point list and it is fully contained
    // in the polygon
    Polygon polygon;
    polygon.edges.reserve(points.size());
    for (int i = 0; i < points.size(); i++) {
        polygon.edges.push_back(Edge{.src = points[i], .dst = points[i + 1]});
    }
    // last edge to the start point
    polygon.edges.push_back(
        Edge{.src = points[points.size() - 1], .dst = points[0]});

    vector<tuple<double, int, int>> points_and_areas;
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            if (polygon.ContainsRectangle(points[i], points[j])) {
                long double area =
                    AreaRectangleBetweenTwoPoints(points[i], points[j]);
                points_and_areas.push_back({area, i, j});
            }
        }
    }

    sort(points_and_areas.begin(), points_and_areas.end());
    reverse(points_and_areas.begin(), points_and_areas.end());
    Point2d p, q;
    p = points[get<1>(points_and_areas[0])];
    q = points[get<2>(points_and_areas[0])];
    long int max_area_as_int = get<0>(points_and_areas[0]);
    cout << "max area " << get<0>(points_and_areas[0]) << " p: " << p.ToString()
         << " q: " << q.ToString() << endl;
    cout << "part 2:" << max_area_as_int << endl;
}

int main() {
    vector<Point2d> points;
    auto res = ReadInput(kInputFname, points);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(points);
    part2(points);
    return 0;
}
