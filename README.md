# CHASE Core Library

The CHASE (Contract-Based Heterogeneous Analysis and Systems Engineering) core library provides the fundamental data structures and algorithms for contract-based design and analysis of cyber-physical systems.

## Features

- **Contract Representation**: Classes for defining system requirements using contracts (assumptions and guarantees).
- **System Modeling**: Representation of systems, components, and their relationships.
- **Logic Support**: Support for various logic formulas (Boolean, Temporal, etc.).
- **Utilities**: Helper functions for graph manipulation, I/O, and simplification.

## Prerequisites

- CMake (version 3.10 or higher)
- C++ Compiler (supporting C++17)
- Python (for bindings)
- pybind11

## Build Instructions

1.  Clone the repository.
2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3.  Configure the project:
    ```bash
    cmake ..
    ```
4.  Build the project:
    ```bash
    make
    ```

## Testing

To run the tests (requires Google Test):

1.  Enable tests during configuration (if not enabled by default):
    ```bash
    cmake -DENABLE_TESTS=ON ..
    ```
2.  Build and run tests:
    ```bash
    make
    ctest
    ```

## License

This project is released under the 3-Clause BSD License.
