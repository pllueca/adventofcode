""" The seat layout fits neatly on a grid. Each position is either floor (.), an empty seat (L),
or an occupied seat (#). 
The following rules are applied to every seat simultaneously:

If a seat is empty (L) and there are no occupied seats adjacent to it, the seat becomes occupied.
If a seat is occupied (#) and four or more seats adjacent to it are also occupied, the seat becomes empty.
Otherwise, the seat's state does not change.

Floor (.) never changes; seats don't move, and nobody sits on the floor.

At this point, something interesting happens: the chaos stabilizes and further applications of these rules cause no seats to change state! Once people stop moving around, you count 37 occupied seats.

Simulate your seating area by applying the seating rules repeatedly until no seats change state. How many seats end up occupied?


"""
from typing import List, Tuple, Callable


def count_neighbors(seat_map: List[List[str]], position: Tuple[int, int]) -> int:
    """ count the occupied neighbors """
    c = 0
    x, y = position
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:
            nx = x + dx
            ny = y + dy
            if (
                (dx, dy) != (0, 0)
                and 0 <= nx < len(seat_map)
                and 0 <= ny < len(seat_map[0])
                and seat_map[nx][ny] == "#"
            ):
                c += 1
    return c


def count_neighbors_greedy(seat_map: List[List[str]], position: Tuple[int, int]) -> int:
    """ count the occupied neighbors. see in a line until empty seat or end """
    c = 0
    x, y = position
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:

            if (dx, dy) == (0, 0):
                continue

            # advance steps until we find an L, and # or we get out
            steps = 1
            while True:
                nx = x + dx * steps
                ny = y + dy * steps
                if 0 <= nx < len(seat_map) and 0 <= ny < len(seat_map[0]):

                    if seat_map[nx][ny] == "#":
                        c += 1
                        break
                    if seat_map[nx][ny] == "L":
                        break
                else:
                    # got to the limit, out
                    break

                steps += 1
    return c


def game_of_seats(
    seat_map: List[List[str]],
    num_neighbors_to_change: int,
    count_func: Callable = count_neighbors,
    debug: bool = False,
) -> int:
    """ return the amount of occupied seats when the game of seats stabilizes """
    occupied_seats = set(
        (i, j)
        for i in range(len(seat_map))
        for j in range(len(seat_map[0]))
        if seat_map[i][j] == "#"
    )
    empty_seats = set(
        (i, j)
        for i in range(len(seat_map))
        for j in range(len(seat_map[0]))
        if seat_map[i][j] == "L"
    )

    while True:

        if debug:
            for l in seat_map:
                print("".join(l))
            print()

        changing_seats = []
        for s in occupied_seats:
            # If a seat is occupied (#) and four or more seats adjacent to it are also occupied,
            # the seat becomes empty.
            if count_func(seat_map, s) >= num_neighbors_to_change:
                changing_seats.append(s)
        for s in empty_seats:
            # if a seat is empty (L) and there are no occupied seats adjacent to it, the seat
            # becomes occupied.
            if count_func(seat_map, s) == 0:
                changing_seats.append(s)

        # update seat_map
        for s in changing_seats:
            x, y = s
            if seat_map[x][y] == "#":
                seat_map[x][y] = "L"
                occupied_seats.remove(s)
                empty_seats.add(s)
            elif seat_map[x][y] == "L":
                seat_map[x][y] = "#"
                empty_seats.remove(s)
                occupied_seats.add(s)
        if not changing_seats:
            # if nothing changed
            break
    return len(occupied_seats)


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        input_map = [[x for x in l.strip()] for l in f.readlines()]
    # print(game_of_seats(input_map))
    print(game_of_seats(input_map, 5, count_neighbors_greedy))
