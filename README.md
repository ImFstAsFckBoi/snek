# 🐍 snek
Small [pybind11](https://github.com/pybind/pybind11) inspired C++17 library for interfacing with the Python API.
Not very complete or correct. Mostly just for practicing the Python API and C++ metaprogramming.

## Usage
It's header only, so just include the [snek/snek.hh](./snek/snek.hh) file.
Then use the `SNEK_ExportMod`and `snek::ExportFn` functions to expose you C++ functions as a Python module. Classes are not implemented yet. 
```cpp
#include "snek/snek.hh"

int mult(int a, int b) {return a * b;}

SNEK_ExportMod(module, "Documentation!...") {
    snek::ExportFn<mult>("mult");
}
```

Compile as you see fit, for example:
```sh
g++ module.cc -o module.so --std=c++17 -shared -fPIC $(pkg-config --cflags python3)
```

Then you should be able to import and use the module in Python.
```python
>>> from module import mult
>>> mult(3, 2)
6
```

## Test

There is a small test that says ` ALL PASSED! ` if everything is correct.
```sh
make && python3 test.py 
```
