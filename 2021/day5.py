# Given colection of lines, count the number of points where vertical or horizontal lines intersect
from typing import List
from collections import defaultdict
from dataclasses import dataclass
import re
import math


@dataclass
class Line:
    x0: int
    y0: int
    x1: int
    y1: int

    def __str__(self) -> str:
        return "{},{} -> {},{}".format(self.x0, self.y0, self.x1, self.y1)

    def is_45_degree(self) -> bool:
        """return true if the line is at a 45 degree angle. this is true if the
        distance in every direction is the same"""
        return abs(self.x1 - self.x0) == abs(self.y1 - self.y0)


def read_input(fpath: str) -> List[Line]:
    # lines are of the form 5,5 -> 8,2
    lines_re = r"^(\d+),(\d+) -> (\d+),(\d+)$"
    lines = []
    with open(fpath, "r") as f:
        for l in f.readlines():
            if m := re.match(lines_re, l):
                lines.append(
                    Line(
                        x0=int(m.group(1)),
                        y0=int(m.group(2)),
                        x1=int(m.group(3)),
                        y1=int(m.group(4)),
                    )
                )
    return lines


def count_vertical_horizontal_intersections(lines: List[Line]) -> int:
    """using only vertical or horizontal lines, count how many points at least
    2 lines overlap"""
    # store the points with a line
    points = defaultdict(lambda: 0)
    dangerous_points = 0
    for l in lines:
        # discard lines that are not vertical or horizontal
        if not (l.x0 == l.x1 or l.y0 == l.y1):
            continue

        if l.x0 == l.x1:
            # points from y0 to y1
            if l.y0 < l.y1:
                points_to_visit = [(l.x0, i) for i in range(l.y0, l.y1 + 1)]
            else:
                points_to_visit = [(l.x0, i) for i in range(l.y1, l.y0 + 1)]
        else:
            # points from x0 to x1
            if l.x0 < l.x1:
                points_to_visit = [(i, l.y0) for i in range(l.x0, l.x1 + 1)]
            else:
                points_to_visit = [(i, l.y0) for i in range(l.x1, l.x0 + 1)]
        for p in points_to_visit:
            points[p] += 1
            if points[p] == 2:
                dangerous_points += 1
    return dangerous_points


def count_vertical_horizontal_diagonal_intersections(lines: List[Line]) -> int:
    """using only vertical, horizontal, or 45 degree angle lines, count how many points at least
    2 lines overlap"""
    # store the points with a line
    points = defaultdict(lambda: 0)
    dangerous_points = 0
    for l in lines:
        if l.x0 == l.x1:
            # points from y0 to y1
            if l.y0 < l.y1:
                points_to_visit = [(l.x0, i) for i in range(l.y0, l.y1 + 1)]
            else:
                points_to_visit = [(l.x0, i) for i in range(l.y1, l.y0 + 1)]
        elif l.y0 == l.y1:
            # points from x0 to x1
            if l.x0 < l.x1:
                points_to_visit = [(i, l.y0) for i in range(l.x0, l.x1 + 1)]
            else:
                points_to_visit = [(i, l.y0) for i in range(l.x1, l.x0 + 1)]
        elif l.is_45_degree():
            # diagonal
            dist = abs(l.y1 - l.y0)
            if l.x0 < l.x1 and l.y0 < l.y1:
                points_to_visit = [(l.x0 + i, l.y0 + i) for i in range(dist + 1)]
            elif l.x0 < l.x1 and l.y0 >= l.y1:
                points_to_visit = [(l.x0 + i, l.y0 - i) for i in range(dist + 1)]
            elif l.x0 >= l.x1 and l.y0 >= l.y1:
                points_to_visit = [(l.x0 - i, l.y0 - i) for i in range(dist + 1)]
            elif l.x0 >= l.x1 and l.y0 < l.y1:
                points_to_visit = [(l.x0 - i, l.y0 + i) for i in range(dist + 1)]

        for p in points_to_visit:
            points[p] += 1
            if points[p] == 2:
                dangerous_points += 1

    return dangerous_points


if __name__ == "__main__":
    # lines = read_input("day5_input_test_2.txt")
    lines = read_input("day5_input.txt")
    print(count_vertical_horizontal_intersections(lines))
    print(count_vertical_horizontal_diagonal_intersections(lines))
