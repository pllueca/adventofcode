""" https://adventofcode.com/2020/day/14
The initialization program (your puzzle input) can either update the bitmask or write a value to memory. Values and memory addresses are both 36-bit unsigned integers. For example, ignoring bitmasks for a moment, a line like mem[8] = 11 would write the value 11 to memory address 8.

The bitmask is always given as a string of 36 bits, written with the most significant bit (representing 2^35) on the left and the least significant bit (2^0, that is, the 1s bit) on the right. The current bitmask is applied to values immediately before they are written to memory: a 0 or 1 overwrites the corresponding bit in the value, while an X leaves the bit in the value unchanged.
 
there are unlimited mem positions ?
"""
from typing import List

from collections import defaultdict
from dataclasses import dataclass
import re

# Group 1 is either mask or mem[XXX], group 2 is XXX, group 3 is the value of the mask
instructions_re = r"(mask|mem\[(\d+)\]) = ([\dX]+)"


class Instruction:
    def __init__(self, name: str, value: str, position: str = None):
        if "mem" in name:
            self.name = "mem"
            self.value = int(value)
            self.position = int(position)
        else:
            self.name = name
            self.value = value

    @property
    def is_mask(self) -> bool:
        return self.name == "mask"

    def __repr__(self) -> str:
        if self.is_mask:
            return f"MASK = {self.value}"
        else:
            return f"MEM[{self.position}] = {self.value}"


def int2b(v: int) -> str:
    """ convert to a 36 bits binary str """
    return f"{v:>036b}"


def replace(s: str, position: int, character: str) -> str:
    """ returns a copy of the string with achanged charachter """
    return s[:position] + character + s[position + 1 :]


def mask_value(v: int, mask: str) -> int:
    """ applies the 36 bits mask to a value """
    binary_v = int2b(v)
    r = ""
    for bit, mask_bit in zip(binary_v, mask):
        if mask_bit == "X":
            r += bit
        else:
            r += mask_bit
    return int(r, 2)


def mask_memory_address(address: int, mask: str) -> str:
    """
    If the bitmask bit is 0, the corresponding memory address bit is unchanged.
    If the bitmask bit is 1, the corresponding memory address bit is overwritten with 1.
    If the bitmask bit is X, the corresponding memory address bit is floating.
    """
    address_bin = int2b(address)
    r = ""
    for addr_bit, mask_bit in zip(address_bin, mask):
        if mask_bit == "0":
            r += addr_bit
        elif mask_bit == "1":
            r += "1"
        else:  # mask_bit == 'X'
            r += "X"  # floating bit
    return r


def run_program(program: List[Instruction]) -> int:
    """ runs the program and returns the sum of the memory contents """
    memory = defaultdict(int)
    current_mask = "X" * 36

    for instruction in program:

        if instruction.is_mask:
            current_mask = instruction.value
        else:
            # compute value thru the mask
            memory[instruction.position] = mask_value(instruction.value, current_mask)

    # after all the instructions are executed, sum the memory contents
    return sum(memory.values())


def unroll_address_inner(address: str, r=[], idx=0) -> None:
    """ a """
    if idx >= len(address):
        r.append(address)
    else:
        if address[idx] != "X":
            unroll_address_inner(address, r, idx + 1)
        else:
            # yield trying 0 and 1]
            unroll_address_inner(replace(address, idx, "1"), r, idx + 1)
            unroll_address_inner(replace(address, idx, "0"), r, idx + 1)


def unroll_address(address: str) -> List[str]:
    l = []
    unroll_address_inner(address, l, 0)
    return l


def run_program_v2(program: List[Instruction]) -> int:
    """ runs the program and returns the sum of the memory contents """
    memory = defaultdict(int)
    current_mask = "X" * 36

    for instruction in program:
        print(instruction)
        print("current mask", current_mask)
        if instruction.is_mask:
            current_mask = instruction.value
        else:
            # compute the value (with floating bits) of the address
            address_with_floating_bit = mask_memory_address(
                instruction.position, current_mask
            )
            print("all addresses affected:")
            for actual_address in unroll_address(address_with_floating_bit):
                print(actual_address)
                memory[actual_address] = instruction.value

    # after all the instructions are executed, sum the memory contents
    return sum(memory.values())


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        data = f.read()

    instructions = []
    for name, position, value in re.findall(instructions_re, data):
        instructions.append(Instruction(name, value, position))
    print(run_program(instructions))
    print(run_program_v2(instructions))
