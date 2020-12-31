"""
In this game, the players take turns saying numbers. They begin by taking turns reading from a list 
of starting numbers (your puzzle input). Then, each turn consists of considering the most recently 
spoken number:
* If that was the first time the number has been spoken, the current player says 0.
* Otherwise, the number had been spoken before; the current player announces how many turns apart 
  the number is from when it was previously spoken.
"""

from typing import List


def game(initial_numbers: List[int], end: int = 2020) -> int:
    used_numbers = {}
    numer_spoken = 0
    # used_numbers[x]= most recent turn x was called
    for x in initial_numbers:
        numer_spoken += 1
        used_numbers[x] = numer_spoken

    next_number = 0
    while True:
        numer_spoken += 1
        if numer_spoken == end:
            return next_number
        if next_number in used_numbers:
            # how many turns apart the number is from when it was previously spoken.
            next_next_number = numer_spoken - used_numbers[next_number]
        else:
            next_next_number = 0
        used_numbers[next_number] = numer_spoken

        next_number = next_next_number


assert game([1, 3, 2]) == 1
assert game([2, 1, 3]) == 10
assert game([1, 2, 3]) == 27
assert game([3, 1, 2]) == 1836

print(game([18, 11, 9, 0, 5, 1], 2020))
print(game([18, 11, 9, 0, 5, 1], 30_000_000))
