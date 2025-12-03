#include <fstream>
#include <iostream>
#include <filesystem>
#include <utility>
#include <vector>
#include <set>

#include "absl/status/status.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

#include "utils/common.h"

using namespace std;
namespace fs = std::filesystem;

// std::string kInputFilename = "2025/input2.txt";
std::string kInputFilename = "2025/input2_full.txt";

typedef pair<long int, long int> Range;

absl::Status ReadInput(
    const string input_filename,
    vector<Range> &ranges)
{

    RETURNERRORIF(
        !fs::exists(input_filename),
        absl::StatusCode::kNotFound,
        "input file doesnt exists");
    ifstream file(input_filename);
    string line;
    // only read once since the input is a single line
    RETURNERRORIF(
        !getline(file, line),
        absl::StatusCode::kNotFound, "error reading input");
    vector<string> input_parts = absl::StrSplit(line, ",");
    for (const string &part : input_parts)
    {
        vector<string> range_start_end = absl::StrSplit(part, "-");
        RETURNERRORIF(
            range_start_end.size() != 2,
            absl::StatusCode::kUnknown, "error reading input");
        long int start, end;
        RETURNERRORIF(
            !absl::SimpleAtoi(string(range_start_end[0]), &start),
            absl::StatusCode::kInvalidArgument,
            "error reading line: " + range_start_end[0]);
        RETURNERRORIF(
            !absl::SimpleAtoi(string(range_start_end[1]), &end),
            absl::StatusCode::kInvalidArgument,
            "error reading line " + range_start_end[1]);
        ranges.push_back(make_pair(start, end));
    }
    return absl::OkStatus();
}

/*
Since the young Elf was just doing silly patterns,
you can find the invalid IDs by looking for any ID which is made only of some sequence of digits repeated twice.
So, 55 (5 twice), 6464 (64 twice), and 123123 (123 twice) would all be invalid IDs.

Your job is to find all of the invalid IDs that appear in the given ranges. In the above example:
Adding up all the invalid IDs in this example produces 1227775554.

*/
void part1(const vector<Range> &ranges)
{
    long int added_invalid_ids = 0;
    for (const Range &range : ranges)
    {
        for (long int id = range.first; id <= range.second; id++)
        {
            // check if id is made *only* of a sequence of digits repeated twice
            string id_as_str = std::to_string(id);
            if (id_as_str.size() % 2 != 0)
            {
                // needs to have even lenght
                continue;
            }
            int half_len = id_as_str.size() / 2;
            string first_half = id_as_str.substr(0, half_len);
            string second_half = id_as_str.substr(half_len, half_len);
            if (first_half == second_half)
            {
                added_invalid_ids += id;
            }
        }
    }
    cout << "Part1: sum of added valid ids is: " << added_invalid_ids << endl;
}

/*
Now, an ID is invalid if it is made only of some sequence of digits repeated at least twice.
So, 12341234 (1234 two times), 123123123 (123 three times), 1212121212 (12 five times), and 1111111 (1 seven times) are all invalid IDs.
*/
void part2(const vector<Range> &ranges)
{
    set<long int> invalid_ids;
    for (const Range &range : ranges)
    {
        for (long int id = range.first; id <= range.second; id++)
        {
            // an ID is invalid if it is made only of some sequence of digits repeated at least twice.
            string id_as_str = std::to_string(id);
            int half_len = id_as_str.size() / 2;
            // from len 1 to half len, check the substr
            for (int substr_len = 1; substr_len <= half_len; substr_len++)
            {
                // if len not divisible by substr_len cant be composed by this
                if ((id_as_str.size() % substr_len) != 0)
                {
                    continue;
                }
                auto pattern = id_as_str.substr(0, substr_len);
                // check if each (at least 1) chunk of size substr_len is the same
                int num_chunks = id_as_str.size() / substr_len;
                bool all_good = true;
                for (int chunk = 1; all_good && chunk < num_chunks; chunk++)
                {
                    // l=1,c=1 -> 1:2
                    // l=3,c=1 -> 3:5
                    int start_idx = chunk * substr_len;
                    string current_chunk_substr = id_as_str.substr(
                        start_idx, substr_len);
                    if (current_chunk_substr != pattern)
                    {
                        all_good = false;
                    }
                }

                if (all_good)
                {
                    invalid_ids.insert(id);
                }
            }
        }
    }
    long int res = 0;
    for (auto &e : invalid_ids)
    {
        res += e;
    }
    cout << "Part2: sum of added valid ids is: " << res << endl;
}

int main()
{
    vector<Range> input;
    absl::Status res_input = ReadInput(kInputFilename, input);
    if (!res_input.ok())
    {
        std::cout << "error reading input from " << kInputFilename << "; Error: " << res_input << endl;
        return -1;
    }
    part1(input);
    part2(input);
    return 0;
}