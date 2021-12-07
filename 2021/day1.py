""" https://adventofcode.com/2021/day/1 """
# To do this, count the number of times a depth measurement increases from the previous measurement.


from typing import List

# part 1
def count_increases(depths: List[int]) -> int:
    s = 0
    for i in range(1, len(depths)):
        if depths[i] > depths[i - 1]:
            s += 1
    return s


def count_increases_window(depths: List[int], window_size: int = 3) -> int:
    """count the increases of a window of size `window_size`.
    count the initial window. every step substract first and add next and cmp
    """
    s = 0
    window = sum(depths[:window_size])
    for i in range(1, len(depths) - window_size + 1):
        # i is start of the new window. substract i-1 and add i + window_size
        new_window = window - depths[i - 1] + depths[i + window_size - 1]
        if new_window > window:
            s += 1

        window = new_window
    return s


def read_input(fpath: str):
    with open(fpath, "r") as f:
        data = f.readlines()
    return [int(x) for x in data if x]


if __name__ == "__main__":
    depths = read_input("day1_input.txt")
    # depths = read_input("day1_input_test.txt")

    print(count_increases(depths))
    print(count_increases_window(depths))
