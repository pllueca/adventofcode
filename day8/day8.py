from typing import List
import re
from dataclasses import dataclass
import copy


@dataclass
class Instruction:
    name: str
    value: int
    executed: bool = False

    def flip(self):
        if self.name == "nop":
            self.name = "jmp"
        elif self.name == "jmp":
            self.name = "nop"
        return self


instruction_re = r"(acc|jmp|nop) ([+-]\d+)"


def run_program_until_loop(program: str) -> int:
    """executes the program and returns the value of the accumulator when an instruction is executed
    a seccond time"""
    instructions = [
        Instruction(name, int(value))
        for (name, value) in re.findall(instruction_re, program)
    ]
    accumulator = 0
    instruction_idx = 0
    while instruction_idx < len(instructions):

        current_instruction = instructions[instruction_idx]
        if current_instruction.executed:
            return accumulator

        if current_instruction.name == "nop":
            instruction_idx += 1
        elif current_instruction.name == "acc":
            accumulator += current_instruction.value
            instruction_idx += 1
        elif current_instruction.name == "jmp":
            instruction_idx += current_instruction.value
        current_instruction.executed = True


class InfiniteLoopException(Exception):
    """ Raised when reaching an infinite loop running a program """


def run_program(instructions: List[Instruction]) -> int:
    """executes the program and returns the value of the accumulator. errors if an instruction is
    visited a 2nd time"""
    accumulator = 0
    instruction_idx = 0
    while instruction_idx < len(instructions):

        current_instruction = instructions[instruction_idx]
        if current_instruction.executed:
            raise InfiniteLoopException(f"Repeated instruction {instruction_idx}")

        if current_instruction.name == "nop":
            instruction_idx += 1
        elif current_instruction.name == "acc":
            accumulator += current_instruction.value
            instruction_idx += 1
        elif current_instruction.name == "jmp":
            instruction_idx += current_instruction.value
        current_instruction.executed = True

    return accumulator


def fix_program(program: str) -> int:
    """Fix the program so that it terminates normally by changing exactly one jmp (to nop) or nop
    (to jmp). What is the value of the accumulator after the program terminates?"""
    # for each jmp or nop, swap them an run the program.
    instructions = [
        Instruction(name, int(value))
        for (name, value) in re.findall(instruction_re, program)
    ]
    instruction_idx = 0
    while instruction_idx < len(instructions):
        # if current instrucion is jmp or np, switch and run.
        current_instruction = instructions[instruction_idx]
        if current_instruction.name in ["nop", "jmp"]:
            instructions_list = copy.deepcopy(instructions)
            modifyied_instructions_list = (
                instructions_list[:instruction_idx]
                + [instructions_list[instruction_idx].flip()]
                + instructions_list[instruction_idx + 1 :]
            )
            try:
                return run_program(modifyied_instructions_list)
            except InfiniteLoopException:
                # there was an infinite loop, this is not the change we need
                pass
        instruction_idx += 1


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        program_str = f.read()
    print(run_program_until_loop(program_str))
    print(fix_program(program_str))
