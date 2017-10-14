# Knapsack Problem
This problem belong into NP-Complete problem because the cost to find
optimal solution is exponential. Suppose we need to generate all possible
solution from N items, then the sum of all choices will be 2^N
combinations.
Using DFS (Depth Breath Search) or BFS (Breath First Search) will cost you
%OMICRON(V + E) where V is all vertex that connected to source vertex and
E is edge that connection all V.

## Modules
### item.py
* Item
Class of set of items.
* buildItems()
Create instances of Item from dummy data set.
* buildRandomItems()
Create N random instance of Item.

### Combinator.py
* powerSet()
Generate possible combinations from given items.
* powerSet2()
Generate possible combinations from given items into two bag in tuple:
(bag1, bag2).
* chainCombination()
Alternative of powerSet() which implemented itertools module.
