""" https://adventofcode.com/2020/day/4 """
from typing import Any
import re


def validate_passport(passport_info: str) -> bool:
    """validate that fields
      byr (Birth Year)
      iyr (Issue Year)
      eyr (Expiration Year)
      hgt (Height)
      hcl (Hair Color)
      ecl (Eye Color)
      pid (Passport ID)
      cid (Country ID)
    are present. cid is optional, all others required. extra fields? repeated?
    """

    # cid is optional
    required_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}
    regex = r"(\w\w\w:)+"
    matches = re.finditer(regex, passport_info, re.MULTILINE)
    field_names = {m.group().replace(":", "") for m in matches}
    missing = len([field for field in required_fields if field not in field_names])
    return missing == 0


def validate_passport_2(passport_info: str) -> bool:
    """rules
    You can continue to ignore the cid field, but each other field has strict rules about what
    values are valid for automatic validation:

    byr (Birth Year) - four digits; at least 1920 and at most 2002.
    iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    hgt (Height) - a number followed by either cm or in:
    If cm, the number must be at least 150 and at most 193.
    If in, the number must be at least 59 and at most 76.
    hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    pid (Passport ID) - a nine-digit number, including leading zeroes.
    cid (Country ID) - ignored, missing or not."""
    # cid is optional

    # validation functions
    def validate_byr(byr: str) -> bool:
        try:
            byr = int(byr)
        except ValueError:
            return False
        return 1920 <= byr <= 2002

    def validate_iyr(iyr: str) -> bool:
        try:
            iyr = int(iyr)
        except ValueError:
            return False
        return 2010 <= iyr <= 2020

    def validate_eyr(eyr: str) -> bool:
        try:
            eyr = int(eyr)
        except ValueError:
            return False
        return 2020 <= eyr <= 2030

    def validate_hgt(hgt: str) -> bool:
        hgt_regex = r"^(\d+)(in|cm)$"
        m = re.match(hgt_regex, hgt)
        if not m:
            return None
        height = int(m.group(1))
        units = m.group(2)
        if units == "cm":
            return 150 <= height <= 193
        else:  # units == in
            return 59 <= height <= 76

    def validate_hcl(hcl: str) -> bool:
        hcl_regex = r"^#\w{6}$"
        return re.match(hcl_regex, hcl) is not None

    def validate_ecl(ecl: str) -> bool:
        return ecl in {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}

    def validate_pid(pid: str) -> bool:
        pid_regex = r"^\d{9}$"
        return re.match(pid_regex, pid) is not None

    required_fields = {
        "byr": validate_byr,
        "iyr": validate_iyr,
        "eyr": validate_eyr,
        "hgt": validate_hgt,
        "hcl": validate_hcl,
        "ecl": validate_ecl,
        "pid": validate_pid,
    }
    regex = r"((\w\w\w):[ \n]?([\w#]+))+"
    matches = re.finditer(regex, passport_info, re.MULTILINE)
    all_fields = []
    for m in matches:
        field_name = m.group(2)
        all_fields.append(field_name)
        field_value = m.group(3)
        # cid is valid wether is present or not
        def_func = lambda cid: True
        if not required_fields.get(field_name, def_func)(field_value):
            return False
    if [field for field in required_fields if field not in all_fields]:
        return False
    return True


def validate_passports(passports_batch: str) -> int:
    return sum(
        validate_passport(passport) for passport in passports_batch.split("\n\n")
    )


def validate_passports_2(passports_batch: str) -> int:
    return sum(
        validate_passport_2(passport) for passport in passports_batch.split("\n\n")
    )


if __name__ == "__main__":
    with open("test_input.txt", "r") as f:
        passport_infos = f.read()
    assert validate_passports(passport_infos) == 2

    with open("test_input_2.txt", "r") as f:
        passport_infos = f.read()
    assert validate_passports_2(passport_infos) == 4

    with open("input.txt", "r") as f:
        passport_infos = f.read()
    print(validate_passports(passport_infos))
    print(validate_passports_2(passport_infos))
