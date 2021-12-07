""" find shinny gold bags """
from typing import List, Tuple, Dict
import re

bag_color_re = r"(\w+ \w+) bags?"

container_bag_re = r"(\w+ \w+) bags contain"
amount_bag_re = r"(\d+) (\w+ \w+)"


def parse_bag_rule(bag_rule: str) -> Tuple[str, List[str]]:
    """parses a rule in the input like
    wavy plum bags contain 3 faded black bags, 4 bright turquoise bag, 5 bright red bags.

    returns the "container" bag and the List of the contained
    """

    bags = re.findall(bag_color_re, bag_rule)

    container = bags[0]
    if bags[1] == "no other bags":
        contained = []
    else:
        contained = bags[1:]
    return container, contained


def parse_bag_rule_amount(bag_rule: str) -> Tuple[str, List[Tuple[int, str]]]:
    """parses a rule in the input like
    wavy plum bags contain 3 faded black bags, 4 bright turquoise bag, 5 bright red bags.

    returns the "container" bag and the List of the contained with the amounts
    """
    container: str = re.findall(container_bag_re, bag_rule)[0]
    contained_res = re.findall(amount_bag_re, bag_rule)
    if not contained_res:
        contained = []
    else:
        contained = [(int(amount), color) for (amount, color) in contained_res]
    return container, contained


def check_bag_contains(
    current_bag: str,
    target_bag: str,
    container_to_contained: Dict[str, List[str]],
    contains_target_bag: Dict[str, bool],
) -> bool:
    """check if current_bag contains target_bag, call recursively. update contains_target_bag
    with the findings"""
    if current_bag not in contains_target_bag:
        for contained_bag in container_to_contained.get(current_bag, []):
            r = check_bag_contains(
                contained_bag, target_bag, container_to_contained, contains_target_bag
            )
            if r:
                contains_target_bag[contained_bag] = True
                return True
            # else:
            # contains_target_bag[contained_bag] = False
        # no found among the containes, then out
        return False
    return contains_target_bag[current_bag]


def count_bag_contains(
    current_bag: str,
    container_to_contained: Dict[str, List[Tuple[int, str]]],
    contains_how_many_bags: Dict[str, int],
) -> int:
    """ counts how many bags contains the current bag """
    if current_bag not in contains_how_many_bags:
        s = 0
        for contained_bag_amout, contained_bag_color in container_to_contained.get(
            current_bag, []
        ):

            inner_contained = count_bag_contains(
                contained_bag_color, container_to_contained, contains_how_many_bags
            )
            s += contained_bag_amout * inner_contained
        contains_how_many_bags[current_bag] = s
    return contains_how_many_bags[current_bag] + 1


def count_golden_bags_containers(bag_rules: List[str]) -> int:
    """
    build a Dict with the all rules
    build a "contains shiny gold bag" Dict that we will maintain
    iterate over each dict and check if contains golden bag.  use contains_target_bag as a DP
    data structure to reuse computations
    """

    target_bag = "shiny gold"

    container_to_contained: Dict[str, List[Tuple[int, str]]] = {}
    contains_target_bag: Dict[str, bool] = {}
    for bag_rule in bag_rules:
        container, contained = parse_bag_rule(bag_rule)
        container_to_contained[container] = contained
        if target_bag in contained:
            contains_target_bag[container] = True

    s = 0
    for bag in container_to_contained:
        if check_bag_contains(
            bag, target_bag, container_to_contained, contains_target_bag
        ):
            s += 1
    return s


def count_golden_bags_contains(bag_rules: List[str]) -> int:
    """ count the total of bags contained in a shiny golden bag """
    target_bag = "shiny gold"

    container_to_contained: Dict[str, List[str]] = {}
    contains_how_many_bags: Dict[str, int] = {}
    for bag_rule in bag_rules:
        container, contained = parse_bag_rule_amount(bag_rule)
        container_to_contained[container] = contained
        if not contained:
            contains_how_many_bags[container] = 0

    # if target_bag is not present error
    assert target_bag in container_to_contained

    return (
        count_bag_contains(target_bag, container_to_contained, contains_how_many_bags)
        - 1
    )


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        rules = f.read().split("\n")[:-1]
    print(count_golden_bags_containers(rules))
    print(count_golden_bags_contains(rules))
