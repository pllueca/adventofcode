"""
Specifically, they need you to find the two entries that sum to 2020 and then multiply those two numbers together.

For example, suppose your expense report contained the following:

1721
979
366
299
675
1456
In this list, the two entries that sum to 2020 are 1721 and 299. Multiplying them together produces 1721 * 299 = 514579, so the correct answer is 514579.

Of course, your expense report is much larger. Find the two entries that sum to 2020; what do you get if you multiply them together?

"""
from typing import List


def sum2020(nums: List[int]) -> int:
    """ find the 2 numbers in the list that add up to 2020 and return the multiplication of them """
    nums.sort()
    i = 0
    j = len(nums) - 1

    while True:
        s = nums[i] + nums[j]
        if s == 2020:
            return nums[i] * nums[j]
        elif s > 2020:
            j -= 1
        else:
            i += 1


def sum_x(nums: List[int], x: int) -> int:
    """find the 2 numbers in the list that add up to x and return the multiplication of them.
    nums is guaranteed to be sorted"""
    i = 0
    j = len(nums) - 1

    while i < j:
        s = nums[i] + nums[j]
        if s == x:
            return nums[i] * nums[j]
        elif s > x:
            j -= 1
        else:
            i += 1

    raise ValueError("not found")


def sum2020_3nums(nums: List[int]) -> int:
    """sameas sum2020 but with 3 numbers
    idea: sort, and fix a number. for each number x, find if 2 nunmbers onthe list add up to 2020-x
    """
    nums.sort()
    for i, x in enumerate(nums):
        # if x > 2020 we discard
        if 2020 - x:
            try:
                # we look for 2 numbers in the list minus index i that add up to 2020-x
                y = sum_x(nums[:i] + nums[i + 1 :], 2020 - x)
                return x * y
            except ValueError:
                pass


# example test
test = [1721, 979, 366, 299, 675, 1456]
assert sum2020(test) == 514579
assert sum2020_3nums(test) == 241861950


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        numbers = [int(x) for x in f.read().split("\n") if x]
    print(sum2020(numbers))
    print(sum2020_3nums(numbers))
