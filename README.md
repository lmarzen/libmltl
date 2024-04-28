# libmltl
A high-performance library for MLTL (Mission-time Linear Temporal Logic) parsing, Abstract Syntax Tree (AST) manipulation, and formula evaluation. Supports C++ and Python interfaces.


## Dependencies

- Make
- GCC
- Python 3
- pybind11

pybind11 is used to create Python bindings for C++ classes. Install pybind11 be installed either as a system package from your linux repository or alternatively via Pip.

For Debian/Ubuntu-based systems:
```bash
sudo apt-get install make gcc python3 python3-dev python3-pybind11
```
For Arch-based systems:
```bash
sudo pacman -S make gcc python3 python3-pybind11
```

pybind11 can alternatively be installed via Pip:
```bash
pip install pybind11
```

## Directory Structure
```
libmltl/
├── examples/            - example C++ and Python files
├── include/             - header files
├── lib/                 - built static library (C++), Python module
├── src/                 - source code
├── tests/
│   ├── regression/      - regression tests
│   └── perf_compare/    - performance benchmark
├── LICENSE              - GLPL v2.1
├── Makefile             - for building
└── README.md            - this file
```

## Build

Running the following command will create (1) `libmltl.a` a static library linkable to C++ code and (2) `libmltl.cpython-*.so` a shared object Python extension module.
These files will be output to libmltl/lib
```bash
make -j
```

### Install

You can choose to install libmltl on your system using
```bash
make -j install
```
optionally supply an install prefix with `PREFIX=`
```bash
make -j install PREFIX=/path/to/install/dir
```
libmltl can be uninstalled using (specify path with `PREFIX=` if applicable)
```bash
make uninstall
```
