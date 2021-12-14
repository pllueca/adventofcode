from queue import PriorityQueue


def read_input(fpath: str) -> list[list[int]]:
    l = []
    with open(fpath, "r") as f:
        for line in f.readlines():
            l.append([int(x) for x in line.strip()])
    return l


def compute_risk_level(heighmap: list[list[int]]) -> int:
    """add the risk level of evey low point (point whose all neighbors are
    higher than itslf)."""
    r = 0
    for i in range(len(heighmap)):
        for j in range(len(heighmap[0])):
            h = heighmap[i][j]
            lowpoint = True
            for di, dj in dirs:
                ni, nj = i + di, j + dj
                if 0 <= ni < len(heighmap) and 0 <= nj < len(heighmap[0]):
                    if heighmap[ni][nj] <= h:
                        lowpoint = False
                        break
            if lowpoint:
                r += h + 1
    return r


dirs = [(0, -1), (-1, 0), (0, 1), (1, 0)]


def find_biggest_basins(heighmap: list[list[int]]) -> int:
    """Find the sie of the 3 bigger basins and multiply it.
    from a lowpoint, explore the map until all surrondoing 9s are reached"""
    basins = PriorityQueue()
    for i in range(len(heighmap)):
        for j in range(len(heighmap[0])):
            h = heighmap[i][j]
            lowpoint = True
            for di, dj in dirs:
                ni, nj = i + di, j + dj
                if 0 <= ni < len(heighmap) and 0 <= nj < len(heighmap[0]):
                    if heighmap[ni][nj] <= h:
                        lowpoint = False
                        break
            if lowpoint:
                basin_size = find_basin_size(heighmap, i, j)
                basins.put(-basin_size)  # negative to order from bigto small
    r = 1
    for _ in range(3):
        r *= -basins.get()
    return r


def find_basin_size(heighmap: list[list[int]], i: int, j: int) -> int:
    """find the size of a basin with a low point at (i,j)"""
    explored = {(i, j)}
    next_positions = [(i, j)]
    basin_size = 0
    while next_positions:
        basin_size += 1
        i, j = next_positions.pop()
        for di, dj in dirs:
            ni, nj = i + di, j + dj
            if (
                0 <= ni < len(heighmap)
                and 0 <= nj < len(heighmap[0])
                and heighmap[ni][nj] != 9
                and (ni, nj) not in explored
            ):
                next_positions.append((ni, nj))
                explored.add((ni, nj))
    return basin_size


if __name__ == "__main__":
    # heightmap = read_input("day9_input_test.txt")
    heightmap = read_input("day9_input.txt")
    print(compute_risk_level(heightmap))
    print(find_biggest_basins(heightmap))
