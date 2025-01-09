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
g++ module.cc -o module.so -shared -fPIC -Isnek/include $(pkg-config --cflags python3)
```

Then you should be able to import and use the module in Python.
```python
>>> from module import mult
>>> mult(3, 2)
6
```
### Install with pip
The nessecary headers can be installed with pip.
```sh
pip install "git+https://github.com/ImFstAsFckBoi/snek"
```

A command to get the include directories, simillar to pkg-config, is included. So now you can compile like this.
```
g++ module.cc -o module.so -shared -fPIC $(python3 -m snek)
```


### Use with setuptools

To build a an extension using snek with setuptools, use the `snek.pkg_config()` command to get the includes. Look at [testmod/setup.py](./testmod/setup.py) for a working example.
```python
from setuptools import Extension
from snek import pkg_config

module = Extension(
    name="module",
    sources=["module.cc"],
    include_dirs=pkg_config(),
)
``` 


## Test

There is a small test that says ` ALL PASSED! ` if everything is correct.
```sh
pip install . ./testmod && python3 test.py 
```
