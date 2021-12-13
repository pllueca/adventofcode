# each fish reproduces every 7th day. when counter goes below 0, their conuter restars to 6 and a new fish with counter of 8 is created

from typing import List
from collections import defaultdict


def count_fish_population(initial_fish: List[int], days: int) -> int:
    """count the amount of fishes every `days` days"""
    fish = initial_fish
    for _ in range(days):
        new_fish = []
        # decreate each one, if 0, make a new one with intial state 8
        for i in range(len(fish)):
            if fish[i] == 0:
                fish[i] = 6
                new_fish.append(8)
            else:
                fish[i] -= 1
        fish += new_fish
    return len(fish)


def count_fish_population_fast(initial_fish: List[int], days: int) -> int:
    """same as count_fish_population, but not storing the whole list, just a counter"""
    fish_by_counter = defaultdict(lambda: 0)
    for f in initial_fish:
        fish_by_counter[f] += 1

    for _ in range(days):
        new_fish = defaultdict(lambda: 0)
        for counter, nfish in fish_by_counter.items():
            if counter == 0:
                # add a new fish with 8 for each fish
                # reset the fishes to 6
                new_fish[6] += nfish
                new_fish[8] += nfish
            else:
                # decrease 1
                new_fish[counter - 1] += nfish
        fish_by_counter = new_fish

    return sum(fish_by_counter.values())


def read_input(path: str) -> List[int]:
    with open(path, "r") as f:
        return [int(x) for x in f.read().split(",")]


if __name__ == "__main__":
    # fish = read_input("day6_input_test.txt")
    fish = read_input("day6_input.txt")
    # print(count_fish_population(fish, 80))
    print(count_fish_population_fast(fish, 80))
    fish = read_input("day6_input.txt")
    print(count_fish_population_fast(fish, 256))
