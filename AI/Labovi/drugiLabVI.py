from typing import List, Tuple, Dict, Callable
from itertools import zip_longest, accumulate, pairwise, starmap, groupby
from functools import reduce


def zad1():

    def poredak_with_itertools(
        lista1: List[int], lista2: List[int]
    ) -> List[Tuple[int, int, str]]:
        return [
            (a, b, "Jeste" if b == 2 * a else "Nije")
            for a, b in zip_longest(lista1, lista2, fillvalue=0)
        ]

    def poredak_without_itertools(
        lista1: List[int], lista2: List[int]
    ) -> List[Tuple[int, int, str]]:
        return [
            (a, b, "Jeste" if b == 2 * a else "Nije")
            for a, b in zip(
                lista1 + [0] * (len(lista2) - len(lista1)),
                lista2 + [0] * (len(lista1) - len(lista2)),
            )
        ]

    res1_itools = poredak_with_itertools([1, 7, 2, 4], [2, 5, 2])
    res1_no_itools = poredak_without_itertools([1, 7, 2, 4], [2, 5, 2])
    print(res1_itools)
    print(res1_no_itools)

    res2_itools = poredak_with_itertools([2, 5, 2], [1, 7, 2, 4])
    res2_no_itools = poredak_without_itertools([2, 5, 2], [1, 7, 2, 4])
    print(res2_itools)
    print(res2_no_itools)


def zad2():

    def spojidict_with_itertools(
        lista1: List[object], lista2: List[object]
    ) -> List[Dict[str, object]]:
        return [
            {"prvi": a, "drugi": b}
            for a, b in zip_longest(lista1, lista2, fillvalue="-")
        ]

    def spojidict_without_itertools(
        lista1: List[object], lista2: List[object]
    ) -> List[Dict[str, object]]:
        return [
            {"prvi": a, "drugi": b}
            for a, b in zip(
                lista1 + ["-"] * (len(lista2) - len(lista1)),
                lista2 + ["-"] * (len(lista1) - len(lista2)),
            )
        ]

    res1_itools = spojidict_with_itertools([1, 7, 2, 4], [2, 5, 2])
    res1_no_itools = spojidict_without_itertools([1, 7, 2, 4], [2, 5, 2])
    print(res1_itools)
    print(res1_no_itools)

    res2_itools = spojidict_with_itertools([2, 5, 2], [1, 7, 2, 4])
    res2_no_itools = spojidict_without_itertools([2, 5, 2], [1, 7, 2, 4])
    print(res2_itools)
    print(res2_no_itools)


def zad3():

    def spoji_with_itertools(
        lista1: List[int], lista2: List[int]
    ) -> List[Tuple[int, int, int]]:
        return [
            (min(a, b), max(a, b), a + b)
            for a, b in zip_longest(lista1, lista2, fillvalue=0)
        ]

    def spoji_without_itertools(
        lista1: List[int], lista2: List[int]
    ) -> List[Tuple[int, int, int]]:
        return [
            (min(a, b), max(a, b), a + b)
            for a, b in zip(
                lista1 + [0] * (len(lista2) - len(lista1)),
                lista2 + [0] * (len(lista1) - len(lista2)),
            )
        ]

    res1_itools = spoji_with_itertools([1, 7, 2, 4], [2, 5, 2])
    res1_no_itools = spoji_without_itertools([1, 7, 2, 4], [2, 5, 2])
    print(res1_itools)
    print(res1_no_itools)

    res2_itools = spoji_with_itertools([2, 5, 2], [1, 7, 2, 4])
    res2_no_itools = spoji_without_itertools([2, 5, 2], [1, 7, 2, 4])
    print(res2_itools)
    print(res2_no_itools)


def zad4():

    def suma(lista: List) -> int:
        return reduce(
            lambda x, y: x + y if isinstance(y, int) else x + suma(y), lista, 0
        )

    res = suma([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    print(res)


def zad5():

    def proizvod(lista1: List[List[int]], lista2: List[int]) -> List[int]:
        return list(
            map(
                lambda podlista, lista2_el: reduce(
                    lambda x, y: x + y,
                    map(lambda podlista_el: podlista_el * lista2_el, podlista),
                ),
                lista1,
                lista2,
            )
        )

    res = proizvod([[1, 2, 3], [4, 5, 6], [7, 8, 9]], [1, 2, 3])
    print(res)


def zad6():

    def objedini(lista1: List[int], lista2: List[int]) -> List[Tuple[int, int]]:
        return [
            (min(a, b), max(a, b)) for a, b in zip_longest(lista1, lista2, fillvalue=0)
        ]

    res1 = objedini([1, 7, 2, 4, 5], [2, 5, 2])
    print(res1)

    res2 = objedini([2, 5, 2], [1, 7, 2, 4, 5])
    print(res2)


def zad7():

    def objedini(lista: List[Tuple]) -> Dict[int, List[int] | None]:
        return {k: v if len(v) > 0 else None for k, *v in lista}

    res = objedini([(1,), (3, 4, 5), (7,), (1, 4, 5), (6, 2, 1, 3)])
    print(res)


def zad8():

    def izracunaj(lista: List) -> List[int]:
        return list(
            map(
                lambda x: x if isinstance(x, int) else reduce(lambda x, y: x * y, x),
                lista,
            )
        )

    res = izracunaj([1, 5, [1, 5, 3], [4, 2], 2, [6, 3]])
    print(res)


def zad9():

    def zamena(lista: List[int], num: int) -> List[int]:
        return [
            (
                a
                if a >= num
                else list(accumulate(reversed(lista), lambda x, y: x + y, initial=0))[
                    len(lista) - i - 1
                ]
            )
            for i, a in enumerate(lista)
        ]

    res1 = zamena([1, 7, 5, 4, 9, 1, 2, 7], 5)
    print(res1)


def zad10():

    def stepen(lista: List[int]) -> List[int]:
        return [a**b for a, b in pairwise(lista)]

    res = stepen([1, 5, 2, 6, 1, 6, 3, 2, 9])
    print(res)


def zad11():

    def proizvod(lista: List) -> int:
        return reduce(
            lambda x, y: x * y if isinstance(y, int) else x * proizvod(y), lista, 1
        )

    res = proizvod([[1, 3, 5], [2, 4, 6], [1, 2, 3]])
    print(res)


def zad12():

    def izracunaj(lista: List) -> List[int]:
        return [
            (
                a * a
                if isinstance(a, int)
                else reduce(lambda x, y: x + y, map(lambda x: x**2, a))
            )
            for a in lista
        ]

    res = izracunaj([2, 4, [1, 2, 3], [4, 2], 2, [9, 5]])
    print(res)


def zad13():

    def skupi(lista: List[List[int]]) -> List[List[int]]:
        return [
            [b + c for b, c in zip_longest(a[0], a[1], fillvalue=0)]
            for a in pairwise(lista)
        ]

    res = skupi([[1, 3, 5], [2, 4, 6], [1, 2]])
    print(res)


def zad14():

    def suma(lista: List[List[int]]) -> int:
        return reduce(
            lambda k, j: k + j, map(lambda x: reduce(lambda z, t: z * t, x), lista)
        )

    res = suma([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    print(res)


def zad15():

    def promeni(lista: List[int], num: int) -> List[int]:
        return list(map(lambda x: x - num if x >= num else x + num, lista))

    res = promeni([7, 1, 3, 5, 6, 2], 3)
    print(res)


def zad16():

    def broj(num: str) -> int:
        return reduce(
            lambda x, y: x + y,
            starmap(
                lambda x, y: int(x, 16) * 16**y,
                [(num[i : i + 2], len(num) - i - 2) for i in range(1, len(num), 2)],
            ),
        )

    res = broj("#FA0EA0")
    print(res)


def zad17():

    def tekst(text: str) -> str:
        return "".join(
            list(
                map(
                    lambda x: (
                        x
                        if 65 <= ord(x) <= 90
                        or 97 <= ord(x) <= 122
                        or 48 <= ord(x) <= 57
                        else "\\u" + f"{hex(ord(x))}"[2:].zfill(4)
                    ),
                    text,
                )
            )
        )

    res = tekst("Otpornost 10Ω.")
    print(res)


def zad18():

    def brojevi(text: str) -> List[int]:
        def get_brojevi(
            text: str, running_int: str, output: List[int]
        ) -> Tuple[str, str, List[int]]:
            if text == "" and running_int == "":
                return ("", "", output)

            elif text == "":
                return (
                    "",
                    "",
                    (
                        output
                        if not running_int.isdigit()
                        else output + [int(running_int)]
                    ),
                )

            return get_brojevi(
                text[1:],
                running_int + text[0] if text[0].isdigit() else "",
                output if text[0].isdigit() else output + [int(running_int)],
            )

        return get_brojevi(text, "", [])[2]

    res = brojevi("42+10=52;10*10=100")
    print(res)


def zad19():

    def brojanje_with_groupby(text: str) -> List[int]:
        return len([1 for _, reps in groupby(text) if len(list(reps)) > 1])

    def brojanje_without_groupby(text: str) -> List[int]:
        def get_count(text: str, running_char: str, count: int) -> Tuple[str, str, int]:
            if text == "":
                return ("", "", count)
            return get_count(
                text[1:], text[0], count + (1 if text[0] == running_char else 0)
            )

        return get_count(text, "", 0)[2]

    res1 = brojanje_with_groupby("aatesttovi")
    print(res1)

    res2 = brojanje_without_groupby("aatesttovi")
    print(res2)


def zad20():

    def izracunaj(lista: List[int], func: Callable) -> List[int]:
        return [
            func(*tup)
            for tup in [
                (lista[i], lista[i + 1], lista[i + 2]) for i in range(len(lista) - 2)
            ]
        ]

    res = izracunaj([2, 5, 1, 6, 7], lambda x, y, z: x + y * z)
    print(res)


def main():
    zad18()


if __name__ == "__main__":
    main()