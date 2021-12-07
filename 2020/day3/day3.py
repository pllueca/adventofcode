"""
example input
..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#

You start on the open square (.) in the top-left corner and need to reach the bottom (below the
bottom-most row on your map).  The toboggan can only follow a few specific slopes (you opted for
a cheaper model that prefers rational numbers); start by counting all the trees you would encounter
for the slope right 3, down 1: From your starting position at the top-left, check the position that
is right 3 and down 1. Then, check the position that is right 3 and down 1 from there, and so on
until you go past the bottom of the map.  the locations you'd check in the above example are marked
here with O where there was an open square and X where there was a tree:

..##.........##.........##.........##.........##.........##.......  --->
#..O#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
.#....X..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
..#.#...#O#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
.#...##..#..X...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
..#.##.......#.X#.......#.##.......#.##.......#.##.......#.##.....  --->
.#.#.#....#.#.#.#.O..#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
.#........#.#........X.#........#.#........#.#........#.#........#
#.##...#...#.##...#...#.X#...#...#.##...#...#.##...#...#.##...#...
#...##....##...##....##...#X....##...##....##...##....##...##....#
.#..#...#.#.#..#...#.#.#..#...X.#.#..#...#.#.#..#...#.#.#..#...#.#  --->
In this example, traversing the map using this slope would cause you to encounter 7 trees.

Starting at the top-left corner of your map and following a slope of right 3 and down 1, how many
trees would you encounter?

"""
from typing import List, Tuple
import functools


def count_trees(pattern: List[str], slope: Tuple[int, int]) -> int:
    """Count the number of trees you would encounter going on a slope of 3 right, down 1 until you
    reach the bottom.  the pattern repeats, on the right"""
    # x = column, y = row. top left is 0, 0
    horizontal_step, vertical_step = slope
    x, y = 0, 0
    tree_count = 0
    w = len(pattern[0])
    h = len(pattern)

    while y < h:
        if pattern[y][x] == "#":
            tree_count += 1
        x += horizontal_step
        # the pattern have limited w but it repeats, so start from the begining when we go past w
        if x >= w:
            x -= w
        y += vertical_step
    return tree_count


def count_trees_various_slopes(pattern: List[str]) -> int:
    """Determine the number of trees you would encounter if, for each of the following slopes,
    you start at the top-left corner and traverse the map all the way to the bottom:

    Right 1, down 1.
    Right 3, down 1.
    Right 5, down 1.
    Right 7, down 1.
    Right 1, down 2.
    In the above example, these slopes would find 2, 7, 3, 4, and 2 tree(s) respectively;
    multiplied together, these produce the answer 336."""

    slopes = [(1, 1), (3, 1), (5, 1), (7, 1), (1, 2)]
    trees_found = [count_trees(pattern, slope) for slope in slopes]
    return functools.reduce(lambda x, y: x * y, trees_found, 1)


test = """..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#""".split(
    "\n"
)

assert count_trees(test, (3, 1)) == 7
assert count_trees_various_slopes(test) == 336

if __name__ == "__main__":
    with open("input.txt", "r") as f:
        pattern = f.read().split("\n")[:-1]
    print(count_trees(pattern, (3, 1)))
    print(count_trees_various_slopes(pattern))
