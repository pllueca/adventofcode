#include "utils.h"
#include <fstream>
#include <iostream>

// std::string input_fname = "2024/input1.txt";
std::string input_fname = "2024/input1_full.txt";

struct InputLists
{
    std::vector<int> l1;
    std::vector<int> l2;
};

void ReadList(const std::string &input_filename, InputLists *lists)
{
    std::ifstream file(input_filename);
    std::string line;
    while (std::getline(file, line))
    {
        auto members = aoc2024::SplitBySpace(line);
        lists->l1.push_back(std::stoi(members[0]));
        lists->l2.push_back(std::stoi(members[1]));
    }
}

/*
To find the total distance between the left list and the right list, add up the distances between all of the pairs you found. In the example above, this is 2 + 1 + 0 + 1 + 2 + 5, a total distance of 11!
*/
void part1(InputLists &input_lists)
{
    std::sort(input_lists.l1.begin(), input_lists.l1.end());
    std::sort(input_lists.l2.begin(), input_lists.l2.end());
    int res = 0;
    for (int i = 0; i < input_lists.l1.size(); i++)
    {
        res += abs(input_lists.l1[i] - input_lists.l2[i]);
    }
    std::cout << "res1: " << res << std::endl;
}

/*
    For each num in list 1, compute how many times appears in list 2,
    then res += number * times it appear
*/
void part2(InputLists &input_lists)
{
    std::sort(input_lists.l1.begin(), input_lists.l1.end());
    std::sort(input_lists.l2.begin(), input_lists.l2.end());

    int res = 0;
    // using 2 pointers; i points to the current number, j points in the second list to the last position visited.
    // we use j to count how many times the target number appears
    int j = 0;
    int target = -1;
    int count;

    for (int i = 0; i < input_lists.l1.size(); i++)
    {
        if (input_lists.l1[i] == target)
        {
            // just counted how many times this number appears, short circuit
            res += (target * count);
            continue;
        }
        if (j >= input_lists.l1.size())
        {
            break;
        }
        target = input_lists.l1[i];
        // increase j until the first apparition of the number
        while (input_lists.l2[j] < target)
        {
            j++;
        }

        count = 0;
        while (input_lists.l2[j] == target)
        {
            count++;
            j++;
        }
        // j is at the first position larger than target in l2
        // std::cout << "number " << target << " appears " << count << " times" << std::endl;
        res += (target * count);
    }
    std::cout << "res2: " << res << std::endl;
}

int main(int argc, char *argv[])
{
    InputLists input_lists;
    ReadList(input_fname, &input_lists);
    part1(input_lists);
    part2(input_lists);
    return 0;
}