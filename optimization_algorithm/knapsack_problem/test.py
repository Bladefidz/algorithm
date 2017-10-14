import combinator
import item

# Test 1
items = item.buildItems()
combos = combinator.powerSet2(items)
for i in combos:
    print(i)

Test 2
items = item.buildRandomItems(1)
combos = combinator.powerSet2(items)
for i in combos:
    print(i)
