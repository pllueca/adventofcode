#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "absl/status/status.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

#include "utils/common.h"

using namespace std;
namespace fs = std::filesystem;
using namespace aoc;

// const string kInputFname = "2025/input3_full.txt";
const string kInputFname = "2025/input3.txt";

absl::Status ReadInput(
    const string input_filename,
    vector<vecInt> &inputs)
{

    RETURNERRORIF(
        !fs::exists(input_filename),
        absl::StatusCode::kNotFound,
        "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    while (getline(file, line))
    {
        vecInt battery_bank;

        for (const char digit : line)
        {
            int digit_as_int;
            RETURNERRORIF(
                !absl::SimpleAtoi(string(1, digit), &digit_as_int),
                absl::StatusCode::kInvalidArgument,
                "error reading digit from line " + line);
            battery_bank.push_back(digit_as_int);
        }
        inputs.push_back(battery_bank);
    }
    return absl::OkStatus();
}

void part1(const vector<vecInt> &batteries)
{
    int total_joltage = 0;
    for (const vecInt &battery : batteries)
    {
        // find the larger number, and the larger at an index after that
        int largest_idx;
        int largest = -1;
        // not checking the last position because we need 2 numbers
        for (int i = 0; i < battery.size() - 1; i++)
        {
            if (battery[i] > largest)
            {
                largest = battery[i];
                largest_idx = i;
            }
        }
        largest = -1;
        int second_largest_idx;
        for (int i = largest_idx + 1; i < battery.size(); i++)
        {
            if (battery[i] > largest)
            {
                largest = battery[i];
                second_largest_idx = i;
            }
        }
        int joltage = (battery[largest_idx] * 10) + battery[second_largest_idx];
        total_joltage += joltage;
    }
    cout << "part 1 total: " << total_joltage << endl;
}

void part2(const vector<vecInt> &batteries)
{

    long int total_joltage = 0;
    for (const vecInt &battery : batteries)
    {
        // same as first but 12
        vecInt idxs(12);
        int last_largest_idx = -1;
        for (int i = 0; i < 12; i++)
        {
            int remaining_digits = 11 - i;
            int largest = -1;
            int largest_idx = -1;
            // find largest idx between last largest, leaving space for at least the remaining digits
            for (int j = last_largest_idx + 1; j < (battery.size() - remaining_digits); j++)
            {
                if (battery[j] > largest)
                {
                    largest = battery[j];
                    largest_idx = j;
                }
            }
            idxs[i] = largest_idx;
            last_largest_idx = largest_idx;
        }

        // idxs contains the indexes, in order
        long int joltage = 0;
        for (int i = 0; i < 12; i++)
        {
            joltage = (joltage * 10) + battery[idxs[i]];
        }
        total_joltage += joltage;
    }
    cout << "part 2 total: " << total_joltage << endl;
}

int main()
{
    vector<vecInt> input;
    auto res = ReadInput(kInputFname, input);
    if (!res.ok())
    {
        cout << "error reading input: " << res << endl;
        return 1;
    }
    part1(input);
    part2(input);
    return 0;
}
