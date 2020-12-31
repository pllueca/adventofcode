from typing import Dict, Tuple, List

import re
import itertools
import functools
import operator

# rules are the form of
# arrival: 1-5 or 7-9
# arrival date: 1-55 or 79-99645
sections_re = r"your ticket:|nearby tickets:"
rules_re = r"[\w ]+: (\d+)-(\d+) or (\d+)-(\d+)"


def parse_rules(rules_text: str) -> Dict[Tuple[int, int], int]:
    """d[3,5] = 1 means that the field 1 have the restriction 3-5 (value of field 1 must be between
    3 and 5"""
    field_ranges_to_rule: Dict[Tuple[int, int], int] = {}

    for i, rule in enumerate(rules_text.strip().split("\n")):
        min1, max1, min2, max2 = re.findall(rules_re, rule)[0]
        field_ranges_to_rule[(int(min1), int(max1))] = i
        field_ranges_to_rule[(int(min2), int(max2))] = i
    return field_ranges_to_rule


def parse_rules2(rules_text: str) -> List[Tuple[int, int, int, int]]:
    """ List([int, int], [int, int]) """
    rules = []

    for i, rule in enumerate(rules_text.strip().split("\n")):
        min1, max1, min2, max2 = re.findall(rules_re, rule)[0]
        rules.append(
            (int(min1), int(max1), int(min2), int(max2)),
        )
    return rules


def count_nearby_tickets_invalid_for_every_field(text: str) -> int:
    rules, _my_ticket, other_tickets = re.split(sections_re, text)
    rules = parse_rules2(rules)
    print(rules[:3])

    s = 0
    for other_ticket in other_tickets.strip().split("\n"):
        for value in other_ticket.split(","):
            value = int(value)
            some_valid = False
            for r1, r2, r3, r4 in rules:
                if not (value < r1 or r2 < value < r3 or value > r4):
                    some_valid = True
                    break
            if not some_valid:
                s += value

    return s


def is_all_valid(
    ticket: List[int], rules: List[Tuple[int, int, int, int]], rules_idx: List[int]
) -> bool:
    """ checks if this combination of the rules is valid """
    for i, idx in enumerate(rules_idx):
        r1, r2, r3, r4 = rules[idx]
        value = ticket[i]
        if value < r1 or r2 < value < r3 or value > r4:
            return False
    return True


def find_fields(text: str) -> int:
    """Once you work out which field is which, look for the six fields on your ticket that start
    with the word departure. What do you get if you multiply those six values together?"""
    rules, my_ticket, other_tickets = re.split(sections_re, text)
    rules = parse_rules2(rules)
    print(rules[:3])

    valid_other_tickets = []
    for other_ticket in other_tickets.strip().split("\n"):
        ticket = [int(x) for x in other_ticket.split(",")]
        for value in ticket:
            some_valid = False
            for r1, r2, r3, r4 in rules:
                if not (value < r1 or r2 < value < r3 or value > r4):
                    some_valid = True
                    break
            if some_valid:
                valid_other_tickets.append(ticket)
    # valid other tickets is
    # try all combinationss?
    for ticket_indexes in itertools.permutations(range(len(rules)), len(rules)):
        # try all tickets
        all_valid = True
        for ticket in valid_other_tickets:
            if not is_all_valid(ticket, rules, ticket_indexes):
                all_valid = False
                break
        if all_valid:
            # found! multiply the values of the fields corresponding to rules 0-5 in your ticket
            departure_values = [int(my_ticket[idx]) for idx in ticket_indexes[:6]]
            return functools.reduce(operator.mul, departure_values)
    raise ValueError("combination ot found!")


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        data = f.read()
    print(count_nearby_tickets_invalid_for_every_field(data))
    print(find_fields(data))
