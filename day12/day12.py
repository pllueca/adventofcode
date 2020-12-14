"""The navigation instructions (your puzzle input) consists of a sequence of single-character actions paired with integer input values. After staring at them for a few minutes, you work out what they probably mean:

Action N means to move north by the given value.
Action S means to move south by the given value.
Action E means to move east by the given value.
Action W means to move west by the given value.
Action L means to turn left the given number of degrees.
Action R means to turn right the given number of degrees.
Action F means to move forward by the given value in the direction the ship is currently facing.
The ship starts by facing east. Only the L and R actions change the direction the ship is facing. (That is, if the ship is facing east and the next instruction is N10, the ship would move north 10 units, but would still move east if the following action were F.)"""


def next_direction_right(direction: str) -> str:
    """ 90 degrees rotation """
    if direction == "N":
        return "E"
    if direction == "E":
        return "S"
    if direction == "S":
        return "W"
    if direction == "W":
        return "N"


def next_direction_left(direction: str) -> str:
    """ 90 degrees rotation """
    if direction == "N":
        return "W"
    if direction == "E":
        return "N"
    if direction == "S":
        return "E"
    if direction == "W":
        return "S"


class Ship:
    def __init__(self):
        self.x = 0  # north-south
        self.y = 0  # west-east
        self.wx = 1
        self.wy = 10
        self.direction = "E"

    def distance_from_origin(self) -> int:
        """ return manhattan distance from (0,0) """
        return abs(self.x) + abs(self.y)

    def action(self, action: str, value: int) -> None:
        """ moce the ship """
        # LR
        if action == "L":
            steps = value // 90
            for _ in range(steps):
                self.direction = next_direction_left(self.direction)
        elif action == "R":
            steps = value // 90
            for _ in range(steps):
                self.direction = next_direction_right(self.direction)
        # NSWEF
        elif action == "N" or action == "F" and self.direction == "N":
            self.x += value
        elif action == "S" or action == "F" and self.direction == "S":
            self.x -= value
        elif action == "E" or action == "F" and self.direction == "E":
            self.y += value
        elif action == "W" or action == "F" and self.direction == "W":
            self.y -= value

    def action2(self, action: str, value: int) -> None:

        if action == "F":
            # move in the waypoint direction
            self.x += self.wx * value
            self.y += self.wy * value

        # move the waypoint
        elif action == "N":
            self.wx += value
        elif action == "S":
            self.wx -= value
        elif action == "W":
            self.wy -= value
        elif action == "E":
            self.wy += value

        # rotate the waypoint
        elif action == "L":
            steps = value // 90
            for _ in range(steps):
                self.rotate_waypoint_left()
        elif action == "R":
            steps = value // 90
            for _ in range(steps):
                self.rotate_waypoint_right()

    def rotate_waypoint_right(self):
        self.wx, self.wy = -self.wy, self.wx

    def rotate_waypoint_left(self):
        self.wx, self.wy = self.wy, -self.wx

    def __str__(self) -> str:
        return f"<Ship at ({self.x}, {self.y}) facing {self.direction}, waypoint ({self.wx, self.wy})>"


if __name__ == "__main__":

    with open("input.txt", "r") as f:
        instructions = f.readlines()

    s = Ship()
    print(s)
    for l in instructions:
        i, v = l[0], int(l[1:])
        s.action(i, v)
        print(s)
    print(s.distance_from_origin())

    s = Ship()
    print(s)
    for l in instructions:
        i, v = l[0], int(l[1:])
        s.action2(i, v)
        print(s)
    print(s.distance_from_origin())
