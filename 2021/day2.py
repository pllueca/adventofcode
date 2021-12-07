from typing import List


def read_input(fpath: str) -> List:
    with open(fpath, "r") as f:
        data = f.readlines()
    return [x.split(" ") for x in data if x]


def calculate_position(movements: List) -> int:
    depth, horizontal = 0, 0
    for movement_type, units in movements:
        units = int(units)
        if movement_type == "forward":
            horizontal += units
        elif movement_type == "down":
            depth += units
        elif movement_type == "up":
            depth -= units

    return depth * horizontal


def calculate_position_part2(movements: List) -> int:
    depth, horizontal, aim = 0, 0, 0
    for movement_type, units in movements:
        units = int(units)
        if movement_type == "forward":
            horizontal += units
            depth += aim * units
        elif movement_type == "down":
            aim += units
        elif movement_type == "up":
            aim -= units

    return depth * horizontal


if __name__ == "__main__":
    movements = read_input("day2_input.txt")
    # movements = read_input("day2_input_test.txt")

    print(calculate_position(movements))
    print(calculate_position_part2(movements))
