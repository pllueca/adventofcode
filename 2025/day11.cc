// from https://adventofcode.com/2025/day/11

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

// string kInputFname = "2025/input11.txt";
string kInputFname = "2025/input11_full.txt";

// input is a collection of nodes with its outputs

struct Node {
    string id;
    vector<string> child_ids;
};

typedef unordered_map<string, Node> Graph;

void PrintGraph(const Graph& g) {
    cout << "<Graph:\n";
    for (const auto& [node_id, node] : g) {
        cout << node_id << " -> ";
        for (const auto& child : node.child_ids) {
            cout << child << ",";
        }
        cout << endl;
    }
    cout << ">\n";
}

absl::Status ReadInput(const string input_filename, Graph& g) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        // each line contains a node definition.
        auto members = SplitBySpace(line);
        // first member is node id, rest is outputs
        // e.g. node1: node2 node3 node4
        RETURNERRORIF(members.size() <= 1, absl::StatusCode::kInvalidArgument,
                      "Incorrect input format: " + line);
        // remove the : at the endo
        string node_id = members[0].substr(0, members[0].size() - 1);
        vector<string> child_ids(members.begin() + 1, members.end());
        g.insert({node_id, {.id = node_id, .child_ids = child_ids}});
    }
    // add the "out" node, for completeness
    g.insert({"out", {.id = "out", .child_ids = {}}});
    return absl::OkStatus();
}

string find0Inflow(const unordered_map<string, int>& inflows) {
    for (auto [node_id, inflow] : inflows) {
        if (inflow == 0) {
            return node_id;
        }
    }

    // should not happen since there is no cycles in the input graph
    assert(false);
}

// return list of node ids, sorted in a way that every node is visted before its
// pointed ones.
vector<string> TopologicalSort(const Graph& g) {
    vector<string> order;
    // count "in" of each vertex
    // remove the one with 0 "in"
    unordered_map<string, int> nodes_inflow;
    for (auto [node_id, node] : g) {
        if (!nodes_inflow.contains(node_id)) {
            nodes_inflow[node_id] = 0;
        }
        for (auto child_node_id : node.child_ids) {
            if (!nodes_inflow.contains(child_node_id)) {
                nodes_inflow[child_node_id] = 1;
            } else {
                nodes_inflow[child_node_id] += 1;
            }
        }
    }
    while (!nodes_inflow.empty()) {
        string next = find0Inflow(nodes_inflow);
        order.push_back(next);
        // remove from the inflows
        nodes_inflow.erase(next);
        // decrease inflow of its neighbors
        const Node& n = g.at(next);
        for (auto neighbor_id : n.child_ids) {
            nodes_inflow[neighbor_id] -= 1;
        }
    }

    return order;
}

// compute every path from the node "you" to the node "out"
void part1(const Graph& g) {
    auto nodes_topological_order = TopologicalSort(g);
    // PRINT_VECTOR(nodes_topological_order);

    unordered_map<string, int> num_paths_to_node;
    for (auto& [node_id, _] : g) {
        num_paths_to_node[node_id] = 0;
    }

    num_paths_to_node["you"] = 1;
    for (string node_id : nodes_topological_order) {
        // update neighbours
        int paths_to_current_node = num_paths_to_node[node_id];
        for (string neighbor_id : g.at(node_id).child_ids) {
            num_paths_to_node[neighbor_id] += paths_to_current_node;
        }
    }

    cout << "part 1: " << num_paths_to_node["out"] << endl;
}

void part2() {}

int main() {
    Graph g;
    auto res = ReadInput(kInputFname, g);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    cout << "read input" << endl;
    // PrintGraph(g);
    part1(g);
    part2();
    return 0;
}
