import testmod

assert testmod.hello() == "HELLO!"
assert testmod.nothing() is None
assert testmod.meaning_of_life() == 42
assert testmod.tup() == (1, 2, 3)
assert testmod.lis() == [1, 2, 3]
assert testmod.mult(3, 2) == 6

print(" ALL PASSED! ")