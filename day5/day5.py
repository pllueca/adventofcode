""" https://adventofcode.com/2020/day/5 """

from typing import List


def seat_position_to_id(seat_position: str) -> int:
    """Instead of zones or groups, this airline uses binary space partitioning to seat people.
    A seat might be specified like FBFBBFFRLR, where F means "front", B means "back", L means
    "left", and R means "right".

    The first 7 characters will either be F or B; these specify exactly one of the 128 rows on the
    plane (numbered 0 through 127). Each letter tells you which half of a region the given seat is
    in. Start with the whole list of rows; the first letter indicates whether the seat is in the
    front (0 through 63) or the back (64 through 127). The next letter indicates which half of that
    region the seat is in, and so on until you're left with exactly one row.

    The last three characters will be either L or R; these specify exactly one of the 8 columns of
    seats on the plane (numbered 0 through 7). The same process as above proceeds again, this time
    with only three steps. L means to keep the lower half, while R means to keep the upper half.

    The last three characters will be either L or R; these specify exactly one of the 8 columns of
    seats on the plane (numbered 0 through 7). The same process as above proceeds again, this time
    with only three steps. L means to keep the lower half, while R means to keep the upper half."""
    row_low = 0
    row_high = 127
    col_low = 0
    col_high = 7
    for c in seat_position:
        if c == "F":
            # lower half of the rows
            row_high = (row_high + row_low) // 2
        elif c == "B":
            # upper half of the rows
            row_low = (row_high + row_low) // 2
        elif c == "L":
            # lower half of the columns
            col_high = (col_high + col_low) // 2
        elif c == "R":
            # upper halg of the columns
            col_low = (col_high + col_low) // 2

    col = col_high
    row = row_high
    return row * 8 + col


def find_my_seat(seat_positions: List[str]) -> int:
    """
    Ding! The "fasten seat belt" signs have turned on. Time to find your seat.

    It's a completely full flight, so your seat should be the only missing boarding pass in your
    list.  However, there's a catch: some of the seats at the very front and back of the plane
    don't exist on this aircraft, so they'll be missing from your list as well.

    Your seat wasn't at the very front or back, though; the seats with IDs +1 and -1 from yours will
    be in your list.
    """

    # sort the ids. find a gap, thats my seat!
    all_seat_ids = sorted([seat_position_to_id(seat) for seat in seat_positions])
    for i, x in enumerate(all_seat_ids):
        if all_seat_ids[i + 1] == x + 2:
            return x + 1


assert seat_position_to_id("BFFFBBFRRR") == 567
assert seat_position_to_id("FFFBBBFRRR") == 119
assert seat_position_to_id("BBFFBBFRLL") == 820

if __name__ == "__main__":
    with open("input.txt", "r") as f:
        rows = f.readlines()

    print(max(seat_position_to_id(seat) for seat in rows))
    print(f"My seat is id {find_my_seat(rows)}")
