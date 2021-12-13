from typing import List

from collections import defaultdict


def read_input(path: str) -> List[int]:
    with open(path, "r") as f:
        return [int(x) for x in f.read().split(",")]


def count_minimum_fuel(crabs: List[int]) -> int:
    """return the minimum amount of fuel need to horizontaly align the crabs"""
    # dummy idea: test every possible alignment from min to max crabs. cout fuel
    # to align to it

    crabs_py_pos = defaultdict(lambda: 0)
    for c in crabs:
        crabs_py_pos[c] += 1

    min_cost_to_align = float("inf")
    for x in range(min(crabs_py_pos.keys()), max(crabs_py_pos.keys()) + 1):
        cost_to_align = 0
        for x_pos, ncrabs in crabs_py_pos.items():
            cost_to_align += abs(x - x_pos) * ncrabs
        # print(f"to pos {x}: {cost_to_align}")
        min_cost_to_align = min(cost_to_align, min_cost_to_align)
    return min_cost_to_align


def count_minimum_fuel_incr(crabs: List[int]) -> int:
    """return the minimum amount of fuel need to horizontaly align the crabs.
    each change of 1 step in horizontal position costs 1 more unit of fuel
    than the last: the first step costs 1, the second step costs 2, the third
    step costs 3, and so on.
    """
    # dummy idea: test every possible alignment from min to max crabs. cout fuel
    # to align to it

    crabs_py_pos = defaultdict(lambda: 0)
    for c in crabs:
        crabs_py_pos[c] += 1

    min_cost_to_align = float("inf")
    for x in range(min(crabs_py_pos.keys()), max(crabs_py_pos.keys()) + 1):
        cost_to_align = 0
        for x_pos, ncrabs in crabs_py_pos.items():
            # moving 1 costs 1, moving 2 costs 3 (1+2), moving 3 costs 6(1+2+3)
            n = abs(x - x_pos)
            movement_cost = n * (n + 1) / 2
            cost_to_align += movement_cost * ncrabs
        # print(f"to pos {x}: {cost_to_align}")
        min_cost_to_align = min(cost_to_align, min_cost_to_align)
    return min_cost_to_align


if __name__ == "__main__":
    crabs = read_input("day7_input_test.txt")
    crabs = read_input("day7_input.txt")
    print(count_minimum_fuel(crabs))
    print(count_minimum_fuel_incr(crabs))
