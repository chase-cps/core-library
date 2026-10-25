# Demonstrations

The demonstrations are available in the [demos][DEMOS] repository, which
contains the following directories:

- **DATE18**: specification files used to produce the experimental results
    presented in the DATE 2018 paper on CHASE.
- [**DATE21**][DATE21]: specification files used during the hands-on tutorial
    presented at the DATE 2021 University Booth. It covers both the DSL-based
    flow (`dsl_tool`) and the logics-based flow (`logics_tool`).
- **CPSWeek22**: demonstration presented at CPS-IoT Week 2022: verification of
    refinement and synthesis of the controllers of a robotic arm, using both a
    single model and a compositional model.
- **RoboticJoints**: an extended version of the robotic arm demonstration.
- **FDL2024**: DSL specification of a small factory (warehouse, conveyors,
    machines, and output bays).

Each directory contains the specification files and, where needed, the scripts
to run the demonstration. The scripts expect the CHASE tools and the solvers
in the `PATH`, as provided by the CHASE Docker image.


[<-- Go back to the main page][HOME]

[HOME]: ../../README.md
[DEMOS]: https://github.com/chase-cps/demos
[DATE21]: https://github.com/chase-cps/demos/blob/main/DATE21/DATE21.md
