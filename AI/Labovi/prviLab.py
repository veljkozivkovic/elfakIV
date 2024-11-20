from algotihms import *
if __name__ == '__main__':
    graph_simple = {
        'A': ['B', 'C'],
        'B': ['D', 'E'],
        'C': ['F', 'G'],
        'D': ['H'],
        'E': ['G', 'I'],
        'F': ['J'],
        'G': ['J'],
        'H': [],
        'I': ['J'],
        'J': []
    }
    graph_simple_with_heuristic = {
        'A': (9, ['B', 'C']),
        'B': (6, ['D', 'E']),
        'C': (7, ['F', 'G']),
        'D': (4, ['H']),
        'E': (8, ['G', 'I']),
        'F': (3, ['J']),
        'G': (4, ['J']),
        'H': (4, []),
        'I': (3, ['J']),
        'J': (0, [])
    }
    print(hill_climbing_search(graph_simple_with_heuristic, 'A', 'J'))


    print(depth_first_search(graph_simple, 'A', 'J'))
    print(depth_first_search_recursive(graph_simple,'A', 'J'))
    print(breath_first_search(graph_simple, 'A', 'J'))
    print('zad1')
    print(zad1(graph_simple, 'A'))
    print('zad2')
    print(zad2(graph_simple, 'A', 'J'))
    print('zad3')
    print(zad3(graph_simple, 'A'))
    print('zad4')
    print(zad4(graph_simple, 'A'))

    graph_wih_dis = {
        'A': ['B'],
        'B': ['A', 'C'],
        'C': ['B'],
        'D': ['E'],
        'E': ['D'],
        'F': []
    }
    graph_neusmeren = {
        'A': ['B', 'C'],
        'B': ['A', 'D', 'E'],
        'C': ['A', 'F'],
        'D': ['B'],
        'E': ['B', 'F'],
        'F': ['C', 'E']
    }
    print('zad5')
    print(zad5(graph_wih_dis))
    print('zad6')
    print(zad6(graph_simple, 'A'))
    print('zad7')
    print(zad7(graph_neusmeren, 'A', 'F'))
    print('zad8')
    print(zad8(graph_neusmeren, 'A', 'E'))
    print('zad9')
    print(zad9(graph_neusmeren,'A', 2))
    graph_zad10 = {
        'A': ['B', 'C'],
        'B': ['A', 'D', 'E'],
        'C': ['A', 'F'],
        'D': ['B'],
        'E': ['B', 'F'],
        'F': ['C', 'E']
    }
    print('zad10')
    print(zad10(graph_zad10, 'A', 'D', 'F'))
    graph = {
        "A": [(1, "B"), (4, "C")],
        "B": [(1, "A"), (2, "D"), (3, "E")],
        "C": [(4, "A"), (1, "F")],
        "D": [(2, "B")],
        "E": [(3, "B"), (1, "F")],
        "F": [(1, "C"), (1, "E")]
    }
    print("zad17")
    print(zad17(graph, 'A'))