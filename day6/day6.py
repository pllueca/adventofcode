""" https://adventofcode.com/2020/day/6 """


def count_yes_answers(group_awnsers: str) -> int:
    answers = set()
    for person_answer in group_awnsers.split("\n"):
        answers.update([q for q in person_answer])
    return len(answers)


def count_everyone_yes_answer(group_awnsers: str) -> int:
    # first answer
    answers = set(group_awnsers.split("\n")[0])

    # for each other answer, keep in `answers` only the present ones
    for other_answer in group_awnsers.split("\n")[1:]:
        # if we discarted all we dont need to continue
        if not answers:
            break
        for c in list(answers):
            if c not in other_answer:
                answers.remove(c)
    return len(answers)


test = """abc

a
b
c

ab
ac

a
a
a
a

b"""

assert count_yes_answers("abc") == 3
assert count_yes_answers("ab\nac") == 3
assert count_yes_answers("a\na\na\na") == 1
assert count_everyone_yes_answer("abc") == 3
assert count_everyone_yes_answer("ab\nac") == 1
assert count_everyone_yes_answer("a\na\na\na") == 1
assert count_everyone_yes_answer("yvf\nyfv\nyfv") == 3

assert (
    sum(count_yes_answers(group_answers) for group_answers in test.split("\n\n")) == 11
)
assert (
    sum(
        count_everyone_yes_answer(group_answers) for group_answers in test.split("\n\n")
    )
    == 6
)

if __name__ == "__main__":
    with open("input.txt", "r") as f:
        all_answers = f.read()

    print(
        sum(
            count_yes_answers(group_answers)
            for group_answers in all_answers.split("\n\n")
        )
    )
    print(
        sum(
            count_everyone_yes_answer(group_answers)
            for group_answers in all_answers.split("\n\n")
        )
    )
