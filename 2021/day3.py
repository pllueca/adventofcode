from typing import List


def compute_power_consumption(report: List[str]) -> int:
    """compute gamma rate and epsilon rate, return multiplication
    gamma is the most common bit in each position. epsilon is the least common
    bit on each position (!gamma)
    """
    gamma = ""
    line_length = len(report[0])
    n = len(report)
    for i in range(line_length):
        # number of ones across the report
        num_1s = sum(report_line[i] == "1" for report_line in report)
        if num_1s > n / 2.0:
            gamma += "1"
        else:
            gamma += "0"
    epsilon = "".join("0" if c == "1" else "1" for c in gamma)

    return int(epsilon, 2) * int(gamma, 2)


def compute_life_support_rating(report: List[str]) -> int:
    """keep numbers by bit criteria until only 1 is left"""

    nums_oxygen = report.copy()
    nums_co2 = report.copy()
    line_length = len(report[0])
    n = len(report)
    for i in range(line_length):
        if len(nums_oxygen) > 1:
            num_1s_oxygen = sum(report_line[i] == "1" for report_line in nums_oxygen)
            target_oxygen = "1" if num_1s_oxygen >= len(nums_oxygen) / 2.0 else "0"
            nums_oxygen = [num for num in nums_oxygen if num[i] == target_oxygen]

        if len(nums_co2) > 1:
            num_1s_co2 = sum(report_line[i] == "1" for report_line in nums_co2)
            target_co2 = "0" if num_1s_co2 >= len(nums_co2) / 2.0 else "1"
            nums_co2 = [num for num in nums_co2 if num[i] == target_co2]

        if len(nums_oxygen) == 1 and len(nums_co2) == 1:
            break

    return int(nums_oxygen[0], 2) * int(nums_co2[0], 2)


def read_input(file: str) -> List[str]:
    with open(file, "r") as f:
        lines = f.readlines()
    return [l.strip() for l in lines if l]


if __name__ == "__main__":
    report = read_input("day3_input.txt")
    # report = read_input("day3_input_test.txt")
    print(compute_power_consumption(report))
    print(compute_life_support_rating(report))
