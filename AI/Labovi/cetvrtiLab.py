from copy import deepcopy

# Figure sa ispravnim koordinatama
#FIGURES = {
#    "G": [(0, 0), (1, 0), (2, 0), (2, 1), (2, 2)],  # Green
#    "B": [(0, 0), (1, 0), (1, 1), (2, 0), (2, 1)],  # Blue
#    "O": [(0, 0), (0, 1), (1, 0), (2, 0), (2, 1)],  # Orange
#    "R": [(0, 1), (1, 0), (1, 1), (1, 2), (2, 1)],  # Red
#    "Y": [(0, 0), (0, 1), (1, 1), (1, 2), (1, 3)],  # Yellow
#}
FIGURES = {
    "G": [(0, 0), (0, 1), (1, 1), (2, 1), (3, 1)],  # crvena
    "B": [(0, 0), (1, 0), (0, 1), (1, 1), (1, 2)],  # zelena
    "O": [(0, 0), (0, 1), (1, 1), (0, 2), (1, 2)],  # ljubicasta
    "R": [(0, 0), (1, 0), (2, 0), (3, 0), (4, 0)],  # narandzasta
    "Y": [(0, 0), (1, 0), (1, 1), (1, 2), (1, 3)]   #plava
}
BOARD_SIZE = 5  # Dimenzije table


class Node:
    def __init__(self, board, figures, steps):
        self.board = board
        self.figures = figures
        self.steps = steps
        self.g = len(steps)  # Broj koraka
        self.h = self.heuristic()  # Heuristika
        self.f = self.g + self.h  # Funkcija vrednovanja

    def heuristic(self):
        # Procentualna popunjenost vrsta
        total_unfilled_percentage = 0
        for row in self.board:
            unfilled_cells = row.count(0)
            row_length = len(row)
            unfilled_percentage = (unfilled_cells / row_length) * 100
            total_unfilled_percentage += unfilled_percentage
        # Vraćamo prosečnu nepokrivenost (može i ukupan zbir za jači uticaj heuristike)
        return total_unfilled_percentage / len(self.board)

    def is_goal(self):
        # Cilj je popuniti sva polja table
        return all(all(cell != 0 for cell in row) for row in self.board)

    def __lt__(self, other):
        return self.f < other.f


class PriorityQueue:
    def __init__(self, max_size):
        self.max_size = max_size
        self.items = [None] * (max_size + 1)
        self.count = 0
        self.indexes = {}

    def is_empty(self):
        return self.count == 0

    def insert(self, element, priority):
        if self.count == self.max_size:
            raise Exception("PriorityQueue is full")

        self.count += 1
        i = self.count
        while i > 1 and priority < self.items[i // 2][1]:
            self.items[i] = self.items[i // 2]
            self.indexes[self.items[i][0]] = i
            i //= 2
        self.items[i] = (element, priority)
        self.indexes[element] = i

    def remove_first(self):
        if self.count == 0:
            raise Exception("Empty PriorityQueue")

        to_return = self.items[1]
        last = self.items[self.count]
        i = 1
        self.count -= 1

        while 2 * i <= self.count:
            child = 2 * i
            if (
                child + 1 <= self.count
                and self.items[child + 1][1] < self.items[child][1]
            ):
                child += 1
            if self.items[child][1] >= last[1]:
                break
            self.items[i] = self.items[child]
            self.indexes[self.items[i][0]] = i
            i = child

        self.items[i] = last
        self.indexes[last[0]] = i
        del self.indexes[to_return[0]]
        return to_return

    def update(self, element, new_priority):
        index = self.indexes[element]
        self.items[index] = (element, new_priority)

        while index > 1 and new_priority < self.items[index // 2][1]:
            self.items[index] = self.items[index // 2]
            self.indexes[self.items[index][0]] = index
            index //= 2

        self.items[index] = (element, new_priority)
        self.indexes[element] = index


# Provera da li je postavljanje figure validno
def is_valid_placement(board, figure, x, y):
    for dx, dy in figure:
        nx, ny = x + dx, y + dy
        if nx < 0 or nx >= BOARD_SIZE or ny < 0 or ny >= BOARD_SIZE:
            return False  # Van granica
        if board[nx][ny] != 0:
            return False  # Preklapanje
    return True


# Postavljanje figure na tablu
def place_figure(board, figure, x, y, figure_id):
    new_board = deepcopy(board)
    for dx, dy in figure:
        nx, ny = x + dx, y + dy
        new_board[nx][ny] = figure_id
    return new_board


# Funkcija za ispis table
def print_board(board):
    for row in board:
        print(" ".join(str(cell) if cell != 0 else "." for cell in row))
    print()


# A* algoritam za rešavanje problema
def solve_with_astar():
    initial_board = [[0] * BOARD_SIZE for _ in range(BOARD_SIZE)]
    initial_node = Node(initial_board, list(FIGURES.keys()), [])
    priority_queue = PriorityQueue(max_size=1000)
    priority_queue.insert(initial_node, initial_node.f)

    while not priority_queue.is_empty():
        current_node = priority_queue.remove_first()[0]

        # Debug: trenutna tabla, koraci i preostale figure
        print("Trenutna tabla:")
        print_board(current_node.board)
        print(f"Koraci: {current_node.steps}")
        print(f"Preostale figure: {current_node.figures}")
        print()

        # Ako je cilj postignut, vraćamo korake
        if current_node.is_goal():
            return current_node.steps

        # Generisanje novih stanja
        for figure_name in current_node.figures:
            figure = FIGURES[figure_name]
            for x in range(BOARD_SIZE):
                for y in range(BOARD_SIZE):
                    if is_valid_placement(current_node.board, figure, x, y):
                        new_board = place_figure(current_node.board, figure, x, y, figure_name)
                        new_figures = current_node.figures.copy()
                        new_figures.remove(figure_name)
                        new_steps = current_node.steps + [(figure_name, x, y)]
                        new_node = Node(new_board, new_figures, new_steps)
                        priority_queue.insert(new_node, new_node.f)

    return None  # Ne postoji rešenje


# Rešavanje problema
solution = solve_with_astar()
if solution:
    print("Rešenje pronađeno:")
    for step in solution:
        print(f"Figura {step[0]} postavljena na poziciju ({step[1]}, {step[2]})")
else:
    print("Nije pronađeno rešenje.")
