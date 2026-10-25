The C++ API provided by the CHASE Library can be accessed by using Python.
The interfacing between the two languages relies on [**pybind**][PYBIND],
while the bindings are defined in the following file:

`pychase/bindings.cc`

The bindings expose the same interface provided by the C++ API to Python
programmers. Thus, refer to the (doxygen-based) documentation of the library
for details about the provided functionalities.

The binding is based on Python 3. The compilation generates the `pychase`
module, which is installed in the following directory:

`$CHASE_INSTALL_DIR/chase/lib/`

The other CHASE modules install their Python modules in the same directory:
`pychase_backends` (chase-backends), `pychase_DSL` (DSL_tool), and
`pychase_logicsLang` (logics_tool).

Thus, it is necessary to set the `PYTHONPATH` environment variable in order to
make the modules visible to the python interpreter. For instance, in Unix-based
systems:

`$> export PYTHONPATH=$PYTHONPATH:$CHASE_INSTALL_DIR/chase/lib/`

`$> python3 -c "import pychase"`

The modules must be used with the same version of the Python interpreter used
to compile them. To choose the interpreter, configure each module with
`-DPython_EXECUTABLE=<path_to_python>`. In the CHASE Docker image, `PYTHONPATH` is already set.


[PYBIND]: https://github.com/pybind/pybind11
