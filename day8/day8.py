import re
from dataclasses import dataclass


@dataclass
class Instruction:
    name: str
    value: int
    executed: bool = False


instruction_re = r"(acc|jmp|nop) ([+-]\d+)"


def run_program(program: str) -> int:
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


if __name__ == "__main__":
    with open("input.txt", "r") as f:
        program = f.read()
    print(run_program(program))
