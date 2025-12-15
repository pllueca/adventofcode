// from https://adventofcode.com/2025/day/10

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>  // Required for std::hash
#include <iostream>
#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

#include "absl/status/status.h"
#include "absl/strings/numbers.h"
#include "utils/common.h"

using namespace std;
using namespace aoc;
namespace fs = std::filesystem;

// std::string kInputFname = "2025/input10.txt";
string kInputFname = "2025/input10_full.txt";

// Custom hash function for std::vector<int>
struct VectorHasher {
    std::size_t operator()(const std::vector<bool>& v) const {
        std::size_t hash = v.size();
        for (bool i : v) {
            // A simple hash combination technique
            hash ^=
                std::hash<int>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

typedef vector<int> Button;

typedef std::unordered_map<std::vector<bool>, int, VectorHasher> BoolVectorMap;

class MachineDefinition {
   public:
    string light_diagram;
    vector<Button> buttons;
    vector<int> joltages;

    int NumLights() const { return light_diagram.size(); }
    vector<bool> LightDiagram() const {
        vector<bool> res;
        for (char c : light_diagram) {
            res.push_back(c == '#');
        }
        return res;
    }
    void Print() {
        cout << "light status: " << light_diagram << endl;
        cout << "buttons: ";
        for (const Button& button : buttons) {
            PRINT_VECTOR(button);
        }
        cout << endl;
        cout << "joltages: ";
        PRINT_VECTOR(joltages);
        cout << endl;
    }
};

typedef vector<MachineDefinition> MachineDefinitions;

absl::Status ReadInput(const string input_filename, MachineDefinitions& input) {
    RETURNERRORIF(!fs::exists(input_filename), absl::StatusCode::kNotFound,
                  "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line)) {
        MachineDefinition current;
        auto members = SplitBySpace(line);
        for (const string& member : members) {
            // (2,3,4) -> 2,3,4
            string comma_separated_values_str =
                member.substr(1, member.size() - 2);
            vector<string> values = SplitByComma(comma_separated_values_str);

            Button new_button;
            int v;
            switch (member[0]) {
                case '[':
                    current.light_diagram = comma_separated_values_str;
                    break;
                case '(':
                    // cast to vector<int>, add a new button
                    new_button = {};
                    for (const string& value : values) {
                        RETURNERRORIF(!absl::SimpleAtoi(value, &v),
                                      absl::StatusCode::kInvalidArgument,
                                      "error reading button: " + member)
                        new_button.push_back(v);
                    }
                    current.buttons.push_back(std::move(new_button));
                    break;
                case '{':
                    for (const string& value : values) {
                        RETURNERRORIF(!absl::SimpleAtoi(value, &v),
                                      absl::StatusCode::kInvalidArgument,
                                      "error reading joltage: " + member)
                        current.joltages.push_back(v);
                    }
                    break;
            }
        }
        input.push_back(current);
    }
    return absl::OkStatus();
}

bool BoolVecEqual(const vector<bool>& v1, const vector<bool>& v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

class Step {
   public:
    Step(int num_steps, int last_pressed_idx, vector<bool> current)
        : num_steps(num_steps),
          last_pressed_idx(last_pressed_idx),
          current(std::move(current)) {}

    int num_steps;
    int last_pressed_idx;
    vector<bool> current;
};

int FewerPressesToActivate(const MachineDefinition& def) {
    vector<bool> state(def.NumLights(), false);
    vector<bool> goal = def.LightDiagram();
    // min nuber of steps to get from current step to goal.
    BoolVectorMap visited;
    queue<Step> steps;
    steps.push(Step(0, -1, state));

    int min_steps = 999;
    while (!steps.empty()) {
        Step current_step = steps.front();
        steps.pop();

        if (visited.contains(current_step.current)) {
            continue;
        }
        visited[current_step.current] = 1;
        if (BoolVecEqual(current_step.current, goal)) {
            min_steps = std::min(min_steps, current_step.num_steps);
            continue;
        }
        for (int button_idx = 0; button_idx < def.buttons.size();
             button_idx++) {
            if (button_idx == current_step.last_pressed_idx) {
                continue;
            }
            auto possible_button = def.buttons[button_idx];
            vector<bool> current_copy = current_step.current;
            for (int button_flip_idx : possible_button) {
                current_copy[button_flip_idx] = !current_copy[button_flip_idx];
            }
            steps.push(
                Step(current_step.num_steps + 1, button_idx, current_copy));
        }
    }
    return min_steps;
}

void part1(const MachineDefinitions& defs) {
    int total_steps = 0;
    for (int i = 0; i < defs.size(); i++) {
        auto def = defs[i];
        // def.Print();
        int steps = FewerPressesToActivate(def);
        total_steps += steps;
        // cout << "min presses: " << steps << endl << endl;
    }
    cout << "part1: " << total_steps << endl;
}
void part2() {}

void PrintInput(const MachineDefinitions& defs) {
    for (int i = 0; i < defs.size(); i++) {
        auto def = defs[i];
        def.Print();
        cout << endl;
        cout << endl;
    }
}

int main() {
    MachineDefinitions defs;
    auto res = ReadInput(kInputFname, defs);
    if (!res.ok()) {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    // PrintInput(defs);
    part1(defs);
    part2();
    return 0;
}
