from typing import List


def plug_adapters(joltages: List[int]) -> int:
    """
    find the order in which to plug *all* the adapters in the list.
    starting joltage is 0
    adapters can take an input 1, 2, or 3 jolts lower than its rating and still produce its rated
    output joltage.

    count the number of 1 jolts differences and the number of 3 jolts differences
    """

    # sort joltages. always pick the closest
    joltages.sort()
    dif1 = 0
    dif3 = 0
    current_joltage = 0
    for adapter in joltages:
        diff = adapter - current_joltage
        if diff == 1:
            dif1 += 1
        elif diff == 3:
            dif3 += 1
        elif diff == 2:
            pass
        else:
            raise Exception("no valid adaptor!")
        current_joltage = adapter

    # there is an extra +3 at the output
    dif3 += 1
    current_joltage += 3
    return dif3 * dif1


def foo(current, joltages):
    # joltages are sorted
    c = 0
    for i in [1,2,3]:


def count_combinations(joltages: List[int]) -> int:
    """To completely determine whether you have enough adapters, you'll need to figure out how many
    different ways they can be arranged. Every arrangement needs to connect the charging outlet to
    your device. The previous rules about when adapters can successfully connect still apply."""
    target_joltage = max(joltages) + 3
    current_joltage = 0
    joltages.sort()
    joltages.insert(0, 0)  # add initial
    print(joltages)

    i = 0
    # we are guaranteed at lesat 1
    c = 1

    for i, current_adaptor in enumerate(joltages):
        # current_adaptor = joltages[i]
        # check how many extra we can fit

        valid_next = 0
        for offset in [1, 2, 3]:
            if i + offset < len(joltages):
                if joltages[i + offset] - current_adaptor <= 3:
                    valid_next += 1
        if valid_next > 1:
            # more than 1 next step
            c *= valid_next
    return c

    # combinations_by_adaptor = {}
    # return count_combinations_backtrack(
    # joltages, target_joltage, current_joltage, combinations_by_adaptor
    # )


if __name__ == "__main__":
    with open("test_input2.txt", "r") as f:
        input_sequence = [int(x) for x in f.read().split("\n") if x]
    print(plug_adapters(input_sequence))
    print(count_combinations(input_sequence))
