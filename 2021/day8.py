from dataclasses import dataclass


@dataclass
class Entry:
    # lenght 10
    signal: list[str]

    # lenght 4
    digits: list[str]

    def num_easy_digits(self) -> int:
        """count the number of times that 1, 4, 7 or 8 appear in the digits
        we can know which are those because
        7 is the only with 3 segments,
        4 is the only that uses 4 segments
        8 is the only that uses 7 segments
        1 is the only that uses 2 segments
        """
        # if uses key then it is value
        segments_to_digit = {3: 7, 4: 4, 7: 8, 2: 1}

        # we dont really need to compute the map, just count how many digits are
        # one of the eassy ones
        total = 0
        for w in self.digits:
            if len(w) in segments_to_digit:
                total += 1
        return total

    def compute_digits_int(self) -> int:
        # compute mapping of digits
        # if uses key then it is value
        segments_to_digit = {3: 7, 4: 4, 7: 8, 2: 1}
        # only "easy" digits
        digit_to_segment_lenght = {
            1: 2,
            4: 4,
            7: 3,
            8: 7,
        }

        mapping = {}
        for w in self.signal:
            if corresponding_number := segments_to_digit.get(len(w)):
                mapping[corresponding_number] = w
        # here we know the mapping for 1, 4, 7 and 8
        # to find the rest of mappings we use heuristics

        remaining = set(x for x in self.signal if x not in mapping.values())
        # 4 - 9 has 4 letter intersection
        digits_9 = [x for x in remaining if len(set(x).intersection(mapping[4])) == 4]
        mapping[9] = digits_9[0]
        remaining.remove(mapping[9])

        # 4 - 2 has 2 letter intersection
        digits_2 = [x for x in remaining if len(set(x).intersection(mapping[4])) == 2]
        mapping[2] = digits_2[0]
        remaining.remove(mapping[2])

        # 0 is the only with lenght 6 that has 2 letter intersection with 1
        # and 3 letter intersection with 7
        digits_0 = [
            x
            for x in remaining
            if (
                len(set(x).intersection(mapping[1])) == 2
                and len(set(x).intersection(mapping[7])) == 3
                and len(x) == 6
            )
        ]
        mapping[0] = digits_0[0]
        remaining.remove(mapping[0])

        # 6 is the only remaining with 6 chars long
        digits_6 = [x for x in remaining if len(x) == 6]
        mapping[6] = digits_6[0]
        remaining.remove(mapping[6])

        # from the remaining (3, 5)
        # 5 has 1 intersection with 1,
        # 3 has 2 insterction with 1
        digits_5 = [x for x in remaining if len(set(x).intersection(mapping[1])) == 1]
        mapping[5] = digits_5[0]
        remaining.remove(mapping[5])

        mapping[3] = remaining.pop()

        inverse_mapping = {"".join(sorted(v)): str(k) for k, v in mapping.items()}
        outputs = [inverse_mapping["".join(sorted(x))] for x in self.digits]

        return int("".join(outputs))


def read_input_2lines(fpath: str) -> list[Entry]:
    l = []
    with open(fpath, "r") as f:
        signal = None
        for line in f.readlines():
            if signal is None:
                # remove trailing separator
                signal = line.strip("| \n").split(" ")
            else:
                digits = line.strip(" \n").split(" ")
                l.append(Entry(signal, digits))
                signal = None

    return l


def read_input(fpath: str) -> list[Entry]:
    l = []
    with open(fpath, "r") as f:
        for line in f.readlines():
            signal, digits = line.split("|")
            l.append(Entry(signal.strip().split(" "), digits.strip(" \n").split(" ")))
    return l


if __name__ == "__main__":
    # entries = read_input("day8_input_test.txt")
    entries = read_input("day8_input.txt")
    print(sum(entry.num_easy_digits() for entry in entries))
    print(sum(entry.compute_digits_int() for entry in entries))
