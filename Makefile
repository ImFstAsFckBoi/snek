CFLAGS =  --std=c++17 -Wall -Wextra $(shell pkg-config --cflags python3) -shared -fPIC

testmod.so: testmod.cc snek.hh
	c++ testmod.cc -o testmod.so ${CFLAGS}

.PHONY: clean
clean:
	rm testmod.so