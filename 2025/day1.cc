#include <cassert>
#include <fstream>
#include <iostream>

#include "utils/common.h"
using namespace std;

struct Instruction
{
    bool left;
    int steps;
};

// std::string kInputFilename = "2025/input1.txt";
string kInputFilename = "2025/input1_full.txt";

void ReadInput(
    const string input_filename,
    vector<Instruction> &instructions)
{
    ifstream file(input_filename);
    string line;
    while (getline(file, line))
    {
        bool left = line[0] == 'L';
        int steps = stoi(
            line.substr(1, line.size()));
        instructions.push_back(Instruction{.left = left, .steps = steps});
    }
}
/*
You could follow the instructions, but your recent required official North Pole secret entrance security training seminar taught you that the safe is actually a decoy. The actual password is the number of times the dial is left pointing at 0 after any rotation in the sequence.
*/
void part1(const vector<Instruction> &input)
{

    int wheel_position = 50;
    int at_zero = 0;
    for (const Instruction &instruction : input)
    {
        int increment = instruction.left ? -instruction.steps : instruction.steps;
        wheel_position = (wheel_position + increment) % 100;
        if (wheel_position < 0)
        {
            wheel_position = 100 + wheel_position;
        }

        if (wheel_position == 0)
        {
            at_zero++;
        }
    }
    cout << "how many times wheel at 0: " << at_zero << endl;
}

void part2(const vector<Instruction> &input)
{

    int wheel_position = 50;
    int crosses_zero = 0;
    for (const Instruction &instruction : input)
    {
        int increment = instruction.left ? -instruction.steps : instruction.steps;
        bool starts_at_zero = wheel_position == 0;

        int loops = abs(increment) / 100;
        crosses_zero += loops;
        increment = increment % 100;
        wheel_position = (wheel_position + increment);
        if (wheel_position < 0)
        {
            wheel_position = 100 + wheel_position;
            if (!starts_at_zero)
            {
                crosses_zero++;
            }
        }
        else if (wheel_position > 99)
        {
            wheel_position = wheel_position % 100;
            crosses_zero++;
        }
        else if (wheel_position == 0)
        {
            crosses_zero++;
        }
    }
    cout << "crosses zero: " << crosses_zero << endl;
}

int main()
{
    vector<Instruction> input;
    ReadInput(kInputFilename, input);
    part1(input);
    part2(input);
    return 0;
}