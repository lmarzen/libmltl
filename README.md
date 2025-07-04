# libmltl
A high-performance library for Mission-time Linear Temporal Logic (MLTL) parsing, Abstract Syntax Tree (AST) manipulation, and formula evaluation. Supports C++ and Python interfaces.

```
libmltl/
├── examples/            - example C++ and Python files
├── include/             - header files
├── lib/                 - built static library (C++), Python module
├── src/                 - source code
├── tests/
│   ├── regression/      - regression tests
│   └── perf_compare/    - performance benchmark
├── LICENSE              - LGPL v2.1
├── Makefile             - for building
└── README.md            - this file
```

## Dependencies

- Make
- GCC
- Python 3
- pybind11

pybind11 is used to create Python bindings for C++ classes. Install pybind11 as either a system package from your linux repository or, alternatively, via Pip.

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

## Build

Running the following command will create (1) `libmltl.a` a static library linkable to C++ code and (2) `libmltl.cpython-*.so` a shared object Python extension module.
These files will be output to libmltl/lib
```bash
make -j
```

Build and run tests with
```bash
make -j tests
```

### Install

You can choose to install libmltl on your system using
```bash
make -j install
```
Optionally supply an install prefix with `PREFIX=`
```bash
make -j install PREFIX=/path/to/install/dir
```

libmltl can be uninstalled using (specify the prefix path with `PREFIX=` if applicable)
```bash
make uninstall
```

## Syntax

The following table lists the precedence and associativity of MLTL operators as interpreted by libmltl. Operators are listed top to bottom, in descending precedence. 

| Precedence | Operator          | Description                                                               | Associativity |
|------------|-------------------|---------------------------------------------------------------------------|---------------|
|      1     | `!` `~` <br> `F[a,b]` <br> `G[a,b]` <br> | Logical negation <br> Temporal finally / eventually <br> Temporal globally / always | Right-to-left |
|      2     | `U[a,b]` <br> `R[a,b]`     | Temporal (strong) until <br> Temporal (weak) release                           | Left-to-right |
|      3     | `&`                 | Logical AND                                                               | Left-to-right |
|      4     | `^`                 | Logical XOR (exclusive or)                                                | Left-to-right |
|      5     | `\|`                | Logical OR (inclusive or)                                                 | Left-to-right |
|      6     | `->`                | Logical implication                                                       | Left-to-right |
|      7     | `<->` `=`             | Logical equivalence                                                       | Left-to-right |

_Note:_ Parentheses can be used to clarify the intended evaluation of a complex compound expression, even if they are not strictly required.

Propositional variables are defined by _pN_ , where _N_ ≥ 0. ex: `p0`, `p1`, `p123`. Propositional constants, _true_ and _false_, are specified using `t`, `tt`, or `true` and `f`, `ff`, or `false`, respectively. Temporal bounds are specified with square brackets, [_a_, _b_], such that 0 ≤ _a_ ≤ _b_. ex:
`G[0,10] p0`, `(p0 & p1) R[3, 6] p1`.

## Usage

See `examples/` for full C++ and Python examples.

### C++
```c++
#include "ast.hh"     // for ASTNode class (used to evaluate traces and manipulate AST)
#include "parser.hh"  // for string and file parsing (includes ast.hh)
```
If you did not install libmltl on your system, you will need to add the following compile flags to tell GCC where to find it.
```makefile
# for relative path:
-Llibmltl/lib # for relative path
-Ilibmltl/include # for relative path
# OR for absolute path:
-L/path/to/libmltl/lib # for absolute path
-I/path/to/libmltl/include # for absolute path
```
Don't forget to specify `-lmltl` when compiling to link with libmltl. See `examples/Makefile` for an example build process.

See `examples/example.cc` for example usage of C++ APIs. For more details, also see `include/ast.hh` and `include/parser.hh`.

### Python
```python
import libmltl
```
If you did not install libmltl on your system, you will need to add the following lines before importing libmltl to tell Python where it can find it.
```python
import os, sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)) + '/libmltl/lib') # for relative path
# sys.path.append('/path/to/libmltl/lib') # for absolute path
```

See `examples/example.py` for example usage of Python APIs. For more details, also see `include/ast.hh` and `include/parser.hh` (the Python interfaces mimic the C++ interfaces).
