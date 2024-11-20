from queue import Queue


def breath_first_search(graph: dict, start: str, end: str) -> list[str]:
    if start == end:
        return [start]
    visited = set()
    visited.add(start)
    parent = {start: None}
    queue = Queue(len(graph))
    queue.put(start)
    while not queue.empty():
        node = queue.get()
        for neighbour in graph[node]:
            if neighbour not in visited:
                visited.add(neighbour)
                parent[neighbour] = node
                queue.put(neighbour)

                if neighbour==end:
                    path = []
                    while neighbour is not None:
                        path.append(neighbour)
                        neighbour = parent[neighbour]
                    return path[::-1]
    return []

def depth_first_search_recursive(graph: dict, start: str, end: str, visited : set[str] = None, path: list[str] = None) -> list[str]:
    if visited is None:
        visited = set()
    if path is None:
        path = []

    visited.add(start)
    path.append(start)

    if start == end:
        return path.copy() # deep copy

    for neighbor in graph[start]:
        if neighbor not in visited:
            result = depth_first_search_recursive(graph, neighbor, end, visited, path)
            if result:
                return result

    path.pop()
    return None



def depth_first_search(graph: dict, start: str, end: str) -> list[str]:
    if start == end:
        return [start]

    visited = set()
    stack = [(start, [start])] # tuple - cvor, put do cvora

    while stack:
        node, path = stack.pop()

        if node == end:
            return path

        if node not in visited:
            visited.add(node)
            for neighbour in graph[node]:
                if neighbour not in visited:
                    stack.append((neighbour, path + [neighbour]))
    return []


'''
Heuristika je procenjena cena puta do konkretnog cvora do cilja
g - cilj
c - cvor
H(g) = 0
H(c) > 0 c!=g
cena puta se odredjuje koliko potega ima izmedju cvora i cilja
cilj heuristike je da se biraju cvorovi koji vise obecavaju da ce stici 
do cilja
'''

def hill_climbing_search(graph: dict[str, tuple[int, list[str]]], start: str, end: str) -> list[str]:
    if start == end:
        return [start]

    stack_nodes = Queue()
    visited = set()
    prev_nodes = {start: None}
    visited.add(start)
    stack_nodes.put(start)
    found_dest = False

    
    while not found_dest and not stack_nodes.empty():
        node = stack_nodes.get()
        destinations = []

        for dest in graph[node][1]:
            element = (graph[dest][0], dest)
            destinations.append(element)

        for dest_heur in sorted(destinations, reverse=True):
            dest_node = dest_heur[1]
            if dest_node not in visited:
                prev_nodes[dest_node] = node
                if dest_node == end:
                    found_dest = True
                    break
                visited.add(dest_node)
                stack_nodes.put(dest_node)

    path = []
    if found_dest:
        path.append(end)
        prev = prev_nodes[end]
        while prev is not None:
            path.append(prev)
            prev = prev_nodes[prev]
        path.reverse()

    return path

'''
1.Napisati funkciju koja određuje 
visinu stabla traženja (broj nivoa u stablu traženja), za algoritam
obilaska grafa po širini, koje se 
formira za zadati polazni čvor i zadati graf. 
OBICAN BFS 
'''
def zad1(graph: dict[str, list[str]], start:str) -> int:
    if start not in graph:
        return -1

    queue = Queue(len(graph))
    visited = set()
    visited.add(start)
    queue.put((start, 1))
    max_level = 0

    while not queue.empty():
        node, level = queue.get()
        max_level = max(max_level, level)

        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.put((neighbor, level+1))

    return max_level

'''
2. Napisati funkciju koja određuje broj disjunktnih puteva između dva zadata čvora u grafu. Rešenje
ne mora biti optimalno prema broju puteva. Dozvoljeno je više puta pozvati algoritam obilaska
grafa. 
'''

def zad2(graph: dict[str, list[str]], start:str, end:str) -> int:
    def dfs(node, target, visited):
        if node == target:
            return 1
        path_count = 0
        visited.add(node)
        for neighbour in graph[node]:
            if neighbour not in visited:
                path_count += dfs(neighbour, target, visited)
        visited.remove(node) #back tracking
        return path_count

    visited = set()
    return dfs(start, end, visited)

'''
3.Napisati funkciju koja na osnovu zadatog usmerenog grafa i zadatog (početnog) čvora S formira
niz čvorova sa njihovim udaljenostima od čvora S. Udaljenost se određuje kao dužina najkraćeg
puta od čvora A do nekog čvora. Dužina puta se određuje kao broj potega koji čine taj put.

'''
def zad3(graph: dict[str, list[str]], start:str) -> list[tuple[str,int]]:

    result = {start: 0}
    queue = Queue(len(graph))
    visited = set()
    queue.put(start)
    visited.add(start)
    while not queue.empty():
        node = queue.get()
        current_distance = result[node]

        for neighbour in graph[node]:
            if neighbour not in visited:
                visited.add(neighbour)
                result[neighbour] = current_distance + 1
                queue.put(neighbour)
    return result

'''
4. Napisati funkciju koja na osnovu zadatog neusmerenog grafa i zadatog (ciljnog) čvora G formira
neusmereni graf sa heuristikom. Heuristika proizvoljnog čvora C se određuje kao udaljenost čvora
C od čvora G. Udaljenost se određuje kao dužina najkraćeg puta između dva čvora. Dužina puta se
određuje kao broj potega koji čine taj put
'''
def zad4(graph: dict[str, list[str]], start: str) -> dict[str, (int, list[str])] :
    result = {start: (0, graph[start])}
    queue = Queue(len(graph))
    visited = set()
    visited.add(start)
    queue.put(start)
    while not queue.empty():
        node = queue.get()
        distance = result[node][0]
        for neighbour in graph[node]:
            if neighbour not in visited:
                visited.add(neighbour)
                queue.put(neighbour)
                result[neighbour]=(distance+1,graph[neighbour])
    return result

'''
5. Napisati funkciju koja ispituje koliko disjunktnih podgrafova postoji u zadatom neusmerenom
grafu. Dozvoljeno je više puta pozvati algoritam obilaska grafa
'''

def zad5(graph: dict[str, list[str]]) -> int:
    num_of_dis_graphs = 0
    visited = set()
    def bfs(start_node):
        queue = Queue()
        queue.put(start_node)
        while not queue.empty():
            node = queue.get()
            for neighbour in graph[node]:
                if neighbour not in visited:
                    visited.add(neighbour)
                    queue.put(neighbour)

    for node in graph:
        if node not in visited:
            num_of_dis_graphs+=1
            bfs(node)

    return num_of_dis_graphs

'''
6. Napisati funkciju koja formira stablo 
traženja za zadati graf, zadati polazni čvor i izabrani algoritam
koji se koristiti za obilazak stabla. Student 
sam bira algoritam za koji se formira stablo traženja
'''
def zad6(graph: dict[str, list[str]], start: str) -> dict[str, str]:
    search_tree = {start: None}
    queue = Queue(len(graph))
    visited = set()
    queue.put(start)
    while not queue.empty():
        node = queue.get()
        visited.add(start)
        for neighbour in graph[node]:
            if neighbour not in visited:
                visited.add(neighbour)
                search_tree[neighbour] = node
                queue.put(neighbour)

    return search_tree

'''
7. Napisati funkciju koja na osnovu zadatog neusmerenog grafa i dva zadata (ciljna) čvora G1 i G2
formira neusmereni graf sa heuristikom. Heuristika proizvoljnog čvora C se određuje kao
udaljenost čvora C do bližeg od čvorova G1 i G2. Udaljenost se određuje kao dužina najkraćeg puta
između dva čvora. Dužina puta se određuje kao broj potega koji čine taj put. Dozvoljeno je najviše
dva puta pozvati prilagođeni algoritam obilaska grafa.
'''
def zad7(graph: dict[str, list[str]], g1: str, g2:str) ->dict[str, tuple[int, list[str]]]:
    def bfs_distances(graph: dict[str, list[str]], start: str) ->dict[str,int]:
        queue = Queue(len(graph))
        distances = {start: 0}
        visited = set()
        queue.put(start)
        while not queue.empty():
            node = queue.get()
            visited.add(node)

            for neighbour in graph[node]:
                if neighbour not in visited:
                    distances[neighbour] = distances[node] + 1
                    queue.put(neighbour)

        return distances

    distances_g1 = bfs_distances(graph, g1)
    distances_g2 = bfs_distances(graph, g2)
    distances = dict()
    for node in graph:
        minimal = min(distances_g1[node], distances_g2[node])
        distances[node] = (minimal, graph[node])

    return distances


'''
8. Napisati funkciju koja određuje put između zadatog polaznog i ciljnog čvora neusmerenog grafa
tako što istovremeno pokreće traženje po širini od polaznog i od ciljnog čvora. Traženje se završava
kada se nađe prvi zajednički čvor za oba traženja. 
'''

def zad8(graph: dict[str, list[str]], start: str, end: str) -> list[str]:
    if start == end:
        return [start]

    
    queue_start = Queue()
    queue_end = Queue()
    queue_start.put(start)
    queue_end.put(end)
    visited_start = {start}
    visited_end = {end}
    parent_start = {start: None}
    parent_end = {end: None}

    intersection_node = None

    while not queue_start.empty() and not queue_end.empty():
        
        if not queue_start.empty():
            node_start = queue_start.get()
            if node_start in visited_end:
                intersection_node = node_start
                break
            for neighbor in graph[node_start]:
                if neighbor not in visited_start:
                    visited_start.add(neighbor)
                    parent_start[neighbor] = node_start
                    queue_start.put(neighbor)

       
        if not queue_end.empty():
            node_end = queue_end.get()
            if node_end in visited_start:
                intersection_node = node_end
                break
            for neighbor in graph[node_end]:
                if neighbor not in visited_end:
                    visited_end.add(neighbor)
                    parent_end[neighbor] = node_end
                    queue_end.put(neighbor)

    
    if intersection_node is not None:
        path = []
        
        node = intersection_node
        while node is not None:
            path.append(node)
            node = parent_start[node]
        path.reverse() 

        
        node = parent_end[intersection_node]
        while node is not None:
            path.append(node)
            node = parent_end[node]

        return path

    
    return None

'''
9. Napisati funkciju koja određuje broj čvorova do kojih može da se dođe od zadatog čvora, tako da
je dužina puta do čvora jednaka zadatoj vrednosti. Obići samo neophodne čvorove. 
'''
def zad9(graph: dict[str, list[str]], start: str, n: int) -> int:
    visited = set()
    queue = Queue(len(graph))
    distances = {start: 0}
    visited.add(start)
    queue.put(start)
    result = 0

    nodes =[]

    while not queue.empty():
        node = queue.get()
        prev = distances[node]
        if prev == n:
            result += 1
            nodes.append(node)
        elif prev > n:
            continue
        for neighbour in graph[node]:
            if neighbour not in visited:
                visited.add(neighbour)
                queue.put(neighbour)
                distances[neighbour] = prev + 1
    return result, nodes

'''
10. Napisati funkciju koja pronalazi put u neusmerenom grafu između dva zadata čvora, pri čemu put
prolazi kroz treći zadati čvor. 
'''

def find_path(graph: dict[str, list[str]], start: str, end: str) -> list[str]:
    queue = Queue()
    queue.put(start)
    parent = {start: None}

    while not queue.empty():
        node = queue.get()

        if node == end:
            path = []
            while node is not None:
                path.append(node)
                node = parent[node]
            path.reverse()
            return path

        for neighbor in graph[node]:
            if neighbor not in parent:
                parent[neighbor] = node
                queue.put(neighbor)

    return None  # Vraća None ako put ne postoji

def zad10(graph: dict[str, list[str]], start: str, mid: str, end: str) ->list[str]:
    path1 = find_path(graph, start, mid)
    if not path1:
        return None

    # Pronađi put od `mid` do `end`
    path2 = find_path(graph, mid, end)
    if not path2:
        return None


    return path1 + path2[1:]

