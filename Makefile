CFLAGS =  --std=c++17 -Wall -Wextra $(shell pkg-config --cflags python3) -shared -fPIC

files = snek/*.hh

testmod.so: testmod.cc $(files)
	c++ testmod.cc -o testmod.so ${CFLAGS}

.PHONY: clean
clean:
	rm testmod.so