#include "utils/common.h"
#include <fstream>
#include <iostream>
#include <format>

using aoc::vecInt;

// std::string input_fname = "2024/input2.txt";
std::string input_fname = "2024/input2_full.txt";

void ReadInput(const std::string &input_filename, std::vector<vecInt> &input_vectors)
{
    std::ifstream file(input_filename);
    std::string line;
    while (std::getline(file, line))
    {
        auto members = aoc2024::SplitBySpace(line);
        vecInt v;
        for (auto &m : members)
        {
            v.push_back(std::stoi(m));
        }
        input_vectors.push_back(v);
    }
}

bool ReportIsSafe(const vecInt &report)
{

    // a report is safe if:
    // The levels are either all increasing or all decreasing.
    // Any two adjacent levels differ by at least one and at most three.
    int current = report[0];
    int decreasing = 0;
    int increasing = 0;
    bool out = false;

    // while we iterate
    // if delta is not between 1 and 3 not safe
    // if delta is between 1 and 3, count wether decreasing or not.
    for (int i = 1; !out && i < report.size(); i++)
    {
        int delta = current - report[i];
        if (delta == 0 || abs(delta) > 3)
        {
            // not safe, either no delta or too large
            out = true;
        }
        else
        {
            if (delta > 0)
            {
                increasing++;
            }
            else
            {
                decreasing++;
            }
        }
        current = report[i];
    }
    // count as safe if we havent found any bad delta and all decreasing or increasing
    return !out && (increasing == 0 || decreasing == 0);
}
void part1(const std::vector<vecInt> &reports)
{
    int safe_reports = 0;
    for (const auto &report : reports)
    {
        if (ReportIsSafe(report))
        {
            safe_reports++;
        }
    }
    std::cout << "res1: " << safe_reports << std::endl;
}

bool reportIsSafeRecursive(const vecInt &report, int i, int previous, bool previous_increasing,
                           int removed_idx)
{
    if (i >= report.size())
    {

        // iterated all the way maybe fixing, maybe not
        std::cout << "is good, " << "has been removed: "
                  << (removed_idx == -1
                          ? -1
                          : report[removed_idx])
                  << " idx: " << removed_idx << std::endl;
        return true;
    }
    int current = report[i];
    int delta = current - previous;
    bool increasing = delta > 0;
    bool failing_condition = delta == 0 || abs(delta) > 3 || increasing != previous_increasing;

    // std::cout << std::format("{} ({}) prev: {}, curr_increasing: {}, prev_increasing: {}, removed: {}, failing?: {}",
    //                          current, i, previous, increasing, previous_increasing, removed_idx, failing_condition)
    //           << std::endl;
    if (
        // already removed a level
        removed_idx != -1 && failing_condition)
    {
        return false;
    }
    if (failing_condition)
    {
        return
            // removing level i, use previous
            reportIsSafeRecursive(report, i + 1, previous, /*previous_increasing=*/previous_increasing, i);
    }

    // not fixed
    return
        // using level i, use current
        reportIsSafeRecursive(report, i + 1, current, /*previous_increasing=*/increasing, removed_idx);
}
// current version fail :s
// most likelly when the one to remove is the first
bool ReportIsSafeRemovingAtMost1(const vecInt &report)
{
    // a report is safe if:
    // The levels are either all increasing or all decreasing.
    // Any two adjacent levels differ by at least one and at most three.
    // and we can remove a bad level

    PRINT_VECTOR(report)
    bool res1 = reportIsSafeRecursive(report, 1, report[0], (report[1] - report[0]) > 0, -1);
    // skipping first level
    bool res2 = reportIsSafeRecursive(report, 2, report[1], (report[2] - report[1]) > 0, 0);

    std::cout << "is " << (res1 || res2 ? "good" : "bad") << std::endl;
    if (!res1 && res2)
    {
        std::cout << "removing first index" << std::endl;
    }
    std::cout << std::endl;
    return res1 || res2;
}

void part2(const std::vector<vecInt> &reports)
{
    int safe_reports = 0;
    for (const auto &report : reports)
    {
        if (ReportIsSafeRemovingAtMost1(report))
        {
            safe_reports++;
        }
    }
    std::cout << "res1: " << safe_reports << std::endl;
}

int main()
{
    std::vector<vecInt> input_vectors;
    ReadInput(input_fname, input_vectors);
    part1(input_vectors);
    part2(input_vectors);
}