import testmod

assert testmod.hello() == "HELLO!"
assert testmod.nothing() is None
assert testmod.tup() == (1, 2, 3)
assert testmod.lis() == [1, 2, 3]

print(" ALL PASSED! ")