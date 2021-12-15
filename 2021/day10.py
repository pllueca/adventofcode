from functools import reduce


def read_input(fpath: str) -> list[int]:
    with open(fpath, "r") as f:
        return [l.strip() for l in f.readlines()]


def count_syntax_errors(line: str) -> int:
    """Find the first illegal character in each corrupted line of the navigation subsystem. What is the total syntax error score for those errors?

    ): 3 points.
    ]: 57 points.
    }: 1197 points.
    >: 25137 points.
    """
    s = []
    open_to_close = {"(": ")", "[": "]", "{": "}", "<": ">"}
    scores = {")": 3, "]": 57, "}": 1197, ">": 25137}
    for c in line:
        if c in open_to_close:
            s.append(c)
        else:
            if not s:
                # incomplete line, ignore
                return 0
            expected = open_to_close[s.pop()]
            if c != expected:
                # corrupted line
                return scores[c]
    # correct or incomplete are both 0 points
    return 0


def count_completion_score(line: str) -> int:
    """start with a total score of 0. Then, for each character, multiply the
    total score by 5 and then increase the total score by the point value given
    for the character in the following table:

    ): 1 point.
    ]: 2 points.
    }: 3 points.
    >: 4 points.
    """
    s = []
    open_to_close = {"(": ")", "[": "]", "{": "}", "<": ">"}
    scores = {")": 1, "]": 2, "}": 3, ">": 4}
    for c in line:
        if c in open_to_close:
            s.append(c)
        else:
            if not s:
                # incomplete line, compute score
                break

            expected = open_to_close[s.pop()]
            if c != expected:
                # corrupted line, no score
                return 0

    # missing closing is ordered inversely as the stack (top of the
    # stack is first that need to be closed)
    missing_closing = [open_to_close[p] for p in s[::-1]]
    return reduce(lambda score, p: (score * 5) + scores[p], missing_closing, 0)


if __name__ == "__main__":
    # lines = read_input("day10_input_test.txt")
    lines = read_input("day10_input.txt")
    print(sum([count_syntax_errors(l) for l in lines]))

    completion_scores = sorted([x for y in lines if (x := count_completion_score(y))])

    print(completion_scores[len(completion_scores) // 2])
