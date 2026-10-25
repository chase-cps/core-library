[<-- Go back to the main page][HOME]

# Requirements

## Docker image

The simplest way to obtain a working installation of CHASE is the Docker image
defined by the `Dockerfile` of this repository. It builds all the CHASE modules
and the external tools (gr1c, slugs, and NuSMV on x86_64):

`$> docker build -t chase .`

`$> docker run --rm -it chase`

The rest of this page describes the installation from sources.

## Compilation and Dependencies
The framework requires the following third party software in order to be built
and executed:

- GCC or Clang toolchain for C++ supporting the C++17 standard
- CMake 3.18+
- Python 3, with its development headers
- [pybind11][PYBIND]: used to build the Python interface
- [ANTLR 4 (ANother Tool for Language Recognition) C++ Runtime library][ANTLR]:
    ANTLR4 is a parser generator for reading, processing, executing, or
    translating structured text or binary files. Its C++ Runtime library is used
    to parse Domain Specific Languages in CHASE. A compatible version is
    provided by the [third_party][THIRD_PARTY] repository.
- Doxygen (optional): to build the API documentation
- Graphviz (optional): to visualize the synthesized controllers

The backends generate input files for the following external tools, which must
be installed separately to analyze the generated specifications:

- [gr1c][GR1C]: a collection of tools for GR(1) synthesis,  used to check
    realizability of GR(1) specifications and synthesize discrete control
    strategies from them.
- [slugs][SLUGS]: a stand-alone reactive synthesis tool for generalized
    reactivity(1) synthesis.
- [NuSMV][NUSMV]: a symbolic model checker, used to verify the
    refinement, consistency, and compatibility of contracts.

## Operating Systems
The framework mostly relies on the C++17 Standard and should be portable to any
architecture and operating system supporting it. CHASE has been tested on:
- Linux Ubuntu 20.04+
- macOS
- Windows 10, using the [Windows Subsystem for Linux][WSL]

# Installation

## Preliminaries

All the libraries, tools, and documentation of CHASE must be installed in the
same directory, given by the `CHASE_INSTALL_DIR` environment variable:

`$> export CHASE_INSTALL_DIR=</absolute/path/to/install/directory>`

In the following, `<path_to_pybind>` is the absolute path to the directory
containing the `pybind11Config.cmake` file of your pybind11 installation.

## Installation order

The modules must be installed in the following order:

1. [third_party][THIRD_PARTY] (ANTLR4 runtime, required by `DSL_tool` and `logics_tool`)
2. [core-library][CORE]
3. [chase-backends][BACKENDS]
4. [DSL_tool][DSL_TOOL] and [logics_tool][LOGICS_TOOL]

## Building a CHASE module

Each module is built with CMake in the same way:

`$> git clone https://github.com/chase-cps/<module>`

`$> cmake -S <module> -B <module>/build -Dpybind11_DIR=<path_to_pybind>`

`$> cmake --build <module>/build -j`

`$> cmake --install <module>/build`

The build type defaults to `Release`; pass `-DCMAKE_BUILD_TYPE=Debug` to
obtain a debug build. When several Python interpreters are installed, pass
`-DPython_EXECUTABLE=<path_to_python>` to choose the one the Python modules
are built for.

Third party dependencies are installed in the `CHASE_INSTALL_DIR/third_party`
directory. The CHASE modules are installed in the `CHASE_INSTALL_DIR/chase`
directory: libraries (both for the C++ and Python interfaces) in
`CHASE_INSTALL_DIR/chase/lib`, header files in `CHASE_INSTALL_DIR/chase/include`,
and executables in `CHASE_INSTALL_DIR/chase/bin`.

## Building the API documentation

Documentation requires Doxygen. From the build directory of the core-library:

`$> make doc`

It will generate HTML and LaTeX documentation in `doc/apidoc`.


[<-- Go back to the main page][HOME]

[SLUGS]: https://github.com/VerifiableRobotics/slugs
[GR1C]: https://github.com/tulip-control/gr1c
[NUSMV]: https://nusmv.fbk.eu/
[ANTLR]: https://www.antlr.org/
[PYBIND]: https://github.com/pybind/pybind11
[WSL]: https://learn.microsoft.com/windows/wsl/
[THIRD_PARTY]: https://github.com/chase-cps/third_party
[CORE]: https://github.com/chase-cps/core-library
[BACKENDS]: https://github.com/chase-cps/chase-backends
[DSL_TOOL]: https://github.com/chase-cps/DSL_tool
[LOGICS_TOOL]: https://github.com/chase-cps/logics_tool
[HOME]: ../../README.md
