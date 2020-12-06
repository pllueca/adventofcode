"""
To try to debug the problem, they have created a list (your puzzle input) of passwords (according to the corrupted database) and the corporate policy when that password was set.

For example, suppose you have the following list:

1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc

Each line gives the password policy and then the password. The password policy indicates the lowest and highest number of times a given letter must appear for the password to be valid. For example, 1-3 a means that the password must contain a at least 1 time and at most 3 times.

In the above example, 2 passwords are valid. The middle password, cdefg, is not; it contains no instances of b, but needs at least 1. The first and third passwords are valid: they contain one a or nine c, both within the limits of their respective policies.

How many passwords are valid according to their policies? 
"""

from typing import List, Tuple
import re

# pattern to parse a line of the password policy
re_pattern = r"(\d+)-(\d+) (\w): (\w+)"


def parse_line(l: str) -> Tuple[int, int, str, str]:
    m = re.match(re_pattern, l)
    if not m:
        raise ValueError(f"invalid line {l}")
    return int(m.group(1)), int(m.group(2)), m.group(3), m.group(4)


def validate_pwd(l: str) -> bool:
    """ parses a line of the password policy and returns wether its valid or not """
    min_occurrences, max_occurrences, letter, pwd = parse_line(l)

    # return wether the number of `letter` in `pwd` is between `min_occurrences` and `max_occurrences`
    num_letter_in_pwd = sum(x == letter for x in pwd)
    return min_occurrences <= num_letter_in_pwd <= max_occurrences


def validate_pwd_new(l: str) -> bool:
    """
      Each policy actually describes two positions in the password, where 1 means the first character, 2 means the second character, and so on. (Be careful; Toboggan Corporate Policies have no concept of "index zero"!) Exactly one of these positions must contain the given letter. Other occurrences of the letter are irrelevant for the purposes of policy enforcement.

    Given the same example list from above:

    1-3 a: abcde is valid: position 1 contains a and position 3 does not.
    1-3 b: cdefg is invalid: neither position 1 nor position 3 contains b.
    2-9 c: ccccccccc is invalid: both position 2 and position 9 contain c.
    How many passwords are valid according to the new interpretation of the policies?"""
    position_1, position_2, letter, pwd = parse_line(l)
    return (pwd[position_1 - 1] == letter or pwd[position_2 - 1] == letter) and not (
        pwd[position_1 - 1] == letter and pwd[position_2 - 1] == letter
    )


def count_valid_pwds(pwd_policies: List[str]) -> int:
    return sum(validate_pwd(policy) for policy in pwd_policies)


def count_valid_pwds_new(pwd_policies: List[str]) -> int:
    return sum(validate_pwd_new(policy) for policy in pwd_policies)


test = ["1-3 a: abcde", "1-3 b: cdefg", "2-9 c: ccccccccc"]

assert count_valid_pwds(test) == 2

assert count_valid_pwds_new(test) == 1

if __name__ == "__main__":
    with open("input.txt", "r") as f:
        pwd_policies = f.readlines()
    print(count_valid_pwds(pwd_policies))
    print(count_valid_pwds_new(pwd_policies))
