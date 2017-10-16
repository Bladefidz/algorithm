###########################
# 6.00.2x Problem Set 1: Space Cows

from ps1_partition import get_partitions
import time

#================================
# Part A: Transporting Space Cows
#================================

def load_cows(filename):
    """
    Read the contents of the given file.  Assumes the file contents contain
    data in the form of comma-separated cow name, weight pairs, and return a
    dictionary containing cow names as keys and corresponding weights as values.

    Parameters:
    filename - the name of the data file as a string

    Returns:
    a dictionary of cow name (string), weight (int) pairs
    """

    cow_dict = dict()

    f = open(filename, 'r')

    for line in f:
        line_data = line.split(',')
        cow_dict[line_data[0]] = int(line_data[1])
    return cow_dict


# Problem 1
def greedy_cow_transport(cows, limit=10):
    """
    Uses a greedy heuristic to determine an allocation of cows that attempts to
    minimize the number of spaceship trips needed to transport all the cows. The
    returned allocation of cows may or may not be optimal.
    The greedy heuristic should follow the following method:

    1. As long as the current trip can fit another cow, add the largest cow that will fit
        to the trip
    2. Once the trip is full, begin a new trip to transport the remaining cows

    Does not mutate the given dictionary of cows.

    Parameters:
    cows - a dictionary of name (string), weight (int) pairs
    limit - weight limit of the spaceship (an int)

    Returns:
    A list of lists, with each inner list containing the names of cows
    transported on a particular trip and the overall list containing all the
    trips
    """
    scw = sorted(cows, key=cows.__getitem__)  # Sorted cow by weight
    cowGroups = []
    picked = []     # Boolean list of picked cow in scw
    cow = None      # Popped cow
    cowGroup = []   # Group of cows to be transported
    curWei = 0      # Weight of current cow group
    for i in range(len(scw)):
        picked.append(False)
    for i in range(len(scw) - 1, -1, -1):
        if (picked[i] == False):
            picked[i] = True
            cow = scw[i]
            curWei = cows.pop(cow)  # Set current group weight
            if (curWei <= limit):
                cowGroup.append(cow)
                for j in range(len(scw) - 1, -1, -1):
                    if (picked[j] == False):
                        cow = scw[j]
                        if (curWei + cows[cow] <= limit):
                            picked[j] = True
                            curWei += cows[cow]
                            cowGroup.append(cow)
                cowGroups.append(cowGroup)
                cowGroup = []
    return cowGroups


# Problem 2
def brute_force_cow_transport(cows,limit=10):
    """
    Finds the allocation of cows that minimizes the number of spaceship trips
    via brute force.  The brute force algorithm should follow the following method:

    1. Enumerate all possible ways that the cows can be divided into separate trips
    2. Select the allocation that minimizes the number of trips without making any trip
        that does not obey the weight limitation

    Does not mutate the given dictionary of cows.

    Parameters:
    cows - a dictionary of name (string), weight (int) pairs
    limit - weight limit of the spaceship (an int)

    Returns:
    A list of lists, with each inner list containing the names of cows
    transported on a particular trip and the overall list containing all the
    trips
    """
    pCow = []
    accept = True
    w = 0
    w1 = []
    w2 = []
    for item in get_partitions(cows):
        for it in item:
            w = 0
            for j in it:
                w += cows[j]
                if w > limit:
                    accept = False
                    break
            if accept:
                w1.append(w)
            else:
                break
        if accept:
            if len(pCow) < 1:
                pCow.append(item)
                w2 = w1
            else:
                if len(w1) <= len(w2):
                    for i in range(len(w1)):
                        if w1[i] < w2[i]:
                            accept = False
                            break
                    if accept:
                        pCow.pop()
                        pCow.append(item)
                        w2 = w1
        accept = True
        w1 = []
    return pCow


# Problem 3
def compare_cow_transport_algorithms():
    """
    Using the data from ps1_cow_data.txt and the specified weight limit, run your
    greedy_cow_transport and brute_force_cow_transport functions here. Use the
    default weight limits of 10 for both greedy_cow_transport and
    brute_force_cow_transport.

    Print out the number of trips returned by each method, and how long each
    method takes to run in seconds.

    Returns:
    Does not return anything.
    """
    print("Greedy algorithms")
    start = time.time()
    greedy_cow_transport(load_cows("ps1_cow_data.txt"), 10)
    end = time.time()
    print(end - start)
    print("Brute algorithms")
    start = time.time()
    brute_force_cow_transport(load_cows("ps1_cow_data.txt"), 10)
    end = time.time()
    print(end - start)


"""
Here is some test data for you to see the results of your algorithms with.
Do not submit this along with any of your answers. Uncomment the last two
lines to print the result of your problem.
"""

# cows = load_cows("ps1_cow_data.txt")
# limit=10
# print(cows)
# print()

# print("Gready")
# print(greedy_cow_transport(cows, limit))
# print()
# print("Brute Force")
# print(brute_force_cow_transport(cows, limit))

compare_cow_transport_algorithms()
