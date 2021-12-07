# get a list of numbers and a collection of bingo boards (always 5x5)
# determine the winning board and compute its score: sum all the unmarked numbers
# in that board and multiply them by the last number called (the one that made won)
from typing import List, Tuple

Board = List[List[int]]


def read_input(fpath: str) -> Tuple[List[int], List[Board]]:
    with open(fpath, "r") as f:
        data = f.read()
    numbers, *boards = data.split("\n\n")
    numbers = [int(x) for x in numbers.split(",")]

    def read_board(board: str) -> Board:
        return [
            [int(x) for x in line.split(" ") if x != ""]
            for line in board.split("\n")
            if line
        ]

    boards = [read_board(b) for b in boards]

    return numbers, boards


def print_board(b: Board):
    print("*****")
    for l in b:
        print(l)
    print("*****")


SIZE = 5


def check_row_col(b: Board, row: int, col: int) -> bool:
    # check if all the elements on row `row` or column `col` are -1
    # check row
    all_col = sum(b[row][ocol] == -1 for ocol in range(SIZE))
    if all_col == SIZE:
        return True

    all_row = sum(b[orow][col] == -1 for orow in range(SIZE))
    if all_row == SIZE:
        return True

    return False


def compute_score(b: Board, last_called: int) -> int:
    # add all non -1 numbers, multipy times last called
    s = 0
    for l in b:
        for x in l:
            # dont add -1s
            s += max(x, 0)
    return s * last_called


def compute_winning_board_score(numbers: List[int], boards: List[Board]) -> int:
    # for each number, cross (convert to -1) its appearances in each board. check
    # lines and columns in each board. if bingo, compute score

    for n in numbers:
        for b in boards:
            # check every position in board. if its equal to n, mark it and check
            # row and column.
            for i in range(SIZE):
                for j in range(SIZE):
                    if b[i][j] == n:
                        b[i][j] = -1
                        if check_row_col(b, i, j):
                            print_board(b)
                            return compute_score(b, n)


def compute_last_winning_board_score(numbers: List[int], boards: List[Board]) -> int:
    # boards that still havent won
    active_boards = set(range(len(boards)))
    for n in numbers:
        for bid in list(active_boards):
            b = boards[bid]
            # check every position in board. if its equal to n, mark it and check
            # row and column.
            for i in range(SIZE):
                for j in range(SIZE):
                    if b[i][j] == n:
                        b[i][j] = -1
                        if check_row_col(b, i, j):
                            if len(active_boards) == 1:
                                # last board to finish!
                                return compute_score(b, n)
                            active_boards.remove(bid)


if __name__ == "__main__":
    numbers, boards = read_input("day4_input_test.txt")
    numbers, boards = read_input("day4_input.txt")
    # print(compute_winning_board_score(numbers, boards))
    print(compute_last_winning_board_score(numbers, boards))
