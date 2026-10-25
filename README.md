# CHASE Core Library

The CHASE (Contract-based Heterogeneous Analysis and Systems Exploration) core library provides the fundamental data structures and algorithms for contract-based design and analysis of cyber-physical systems.

## Features

- **Contract Representation**: Classes for defining system requirements using contracts (assumptions and guarantees).
- **System Modeling**: Representation of systems, components, and their relationships.
- **Logic Support**: Support for various logic formulas (Boolean, Temporal, etc.).
- **Utilities**: Helper functions for graph manipulation, I/O, and simplification.
- **Python Bindings**: The C++ API is available in Python through the `pychase` module.

## Prerequisites

- CMake (version 3.18 or higher)
- C++ Compiler (supporting C++17)
- Python 3 (for bindings)
- pybind11

## Build Instructions

1.  Clone the repository.
2.  Set the installation directory of CHASE:
    ```bash
    export CHASE_INSTALL_DIR=/absolute/path/to/install/directory
    ```
3.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
4.  Configure the project:
    ```bash
    cmake .. -Dpybind11_DIR=<path_to_pybind>
    ```
5.  Build and install the project:
    ```bash
    make
    make install
    ```

The project is compiled in `Release` mode by default. To compile it in `Debug`
mode, add `-DCMAKE_BUILD_TYPE=Debug` to the `cmake` command. When several
Python interpreters are installed, add `-DPython_EXECUTABLE=<path_to_python>`
to choose the one the Python bindings are built for.

For the installation of the other CHASE modules, please refer to the
[installation instructions](doc/md/installation.md) or to the
[CHASE website](https://chase-cps.github.io).

## Docker

The `Dockerfile` in this repository builds an image containing all the CHASE
modules and the external tools used by the backends (gr1c, slugs, and NuSMV).
From the main directory of the repository:

```bash
docker build -t chase .
docker run --rm -it chase
```

The container starts in the directory of the [CHASE demos](https://github.com/chase-cps/demos).
NuSMV is distributed only for x86_64; on ARM machines (e.g., Apple Silicon),
add `--platform linux/amd64` to both commands to have it.

## Testing

Tests are enabled by default and require Google Test, which is downloaded
during the configuration.

1.  To disable the tests, configure the project with:
    ```bash
    cmake -DENABLE_TESTS=OFF ..
    ```
2.  To build and run the tests:
    ```bash
    make
    ctest
    ```

## Documentation

- [Installation](doc/md/installation.md)
- [Tools](doc/md/tools.md)
- [Python interface](doc/md/python.md)
- [Representation library](doc/md/library.md)
- [Demonstrations](doc/md/demo.md)

## License

This project is released under the 3-Clause BSD License.
