from itertools import chain, combinations

def powerSet(items):
    """
        Generate all combinations of N items.

        Yield combinations of contains in a bag as list.
        Time complexity is 2**N because we created list of binary bits
        (1 or 0).
    """
    N = len(items)
    # enumerate the 2**N possible combinations
    for i in range(2**N):
        combo = []
        for j in range(N):
            # test bit jth of integer i
            if (i >> j) % 2 == 1:
                combo.append(items[j])
        yield combo

def powerSet2(items):
    """
        Generates all combinations of N items into two bags, whereby each
        item is in one or zero bags.

        Yields a tuple, (bag1, bag2), where each bag is represented as a
        list of which item(s) are in each bag.
        Time complexity is 3**N because we create list of trinary bits
        (0, 1 or 2).
    """
    N = len(items)
    # Enumerate the 3**N possible combinations
    for i in range(3**N):
        bag1 = []
        bag2 = []
        for j in range(N):
            if (i // (3 ** j)) % 3 == 1:
                bag1.append(items[j])
            elif (i // (3 ** j)) % 3 == 2:
                bag2.append(items[j])
        yield (bag1, bag2)

def chainCombination(items):
    """
        Return possible combinations of given items as tuples.
    """
    return chain.from_iterable(combinations(items, r) for r in range(len(items) + 1))
