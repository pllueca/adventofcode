""" https://adventofcode.com/2020/day/9

XMAS starts by transmitting a preamble of 25 numbers. After that, each number you receive should be
the sum of any two of the 25 immediately previous numbers. The two numbers will have different
values, and there might be more than one such pair.
"""
from typing import List

PREAMBLE_LENGTH = 25


def sum_of_2_prev(value: int, nums: List[int]) -> bool:
    """find the 2 numbers in the list that add up to x, if so."""
    nums = sorted(nums)
    i = 0
    j = len(nums) - 1

    while i < j:
        s = nums[i] + nums[j]
        if s == value:
            return True
        elif s > value:
            j -= 1
        else:
            i += 1
    return False


def first_incorrect_in_sequence(sequence: List[int]) -> int:
    previous = sequence[:PREAMBLE_LENGTH]
    i = PREAMBLE_LENGTH
    while True:
        # sum of 2 prev sorts the list, but probably we can just sort it at the begining and instert
        # the new values in the appropiate position
        if not sum_of_2_prev(sequence[i], previous):
            # print(f"error at index {i}")
            return sequence[i]
        previous = previous[1:] + [sequence[i]]
        i += 1


def break_weakness(sequence: List[int]) -> int:
    """The final step in breaking the XMAS encryption relies on the invalid number you just found:
    you must find a contiguous set of at least two numbers in your list which sum to the invalid
    number from step 1.
    To find the encryption weakness, add together the smallest and largest number in this contiguous
    range; in this example, these are 15 and 47, producing 62.
    """
    target = first_incorrect_in_sequence(sequence)

    # fist a continguous set of numbers in sequence that add up to `target`

    nums = sequence[:2]
    current_sum = sum(nums)
    initial_idx = 0
    offset = 2
    # print(f"target is {target}")
    while True:
        # print(f"{nums} -> {current_sum}")
        # print(f"from {initial_idx} to {initial_idx+offset} (offset {offset})")

        if current_sum == target:
            # found it!
            break

        if current_sum > target:
            # too much, restart at next initial index
            initial_idx += 1
            offset = 2
            nums = sequence[initial_idx : initial_idx + offset]
            current_sum = sum(nums)
        else:
            # keep going!
            nums.append(sequence[initial_idx + offset])
            offset += 1
            current_sum += nums[-1]

    print(f"sequence that adds up to target ({target}): {nums})")
    return min(nums) + max(nums)


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        input_sequence = [int(x) for x in f.read().split("\n") if x]
        # print(input_sequence[:10])
    # print(first_incorrect_in_sequence(input_sequence))
    print(break_weakness(input_sequence))
