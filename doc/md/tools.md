[<-- Go back to the main page][HOME]

# Tools

After building and installing the CHASE modules, the following tools are
available in the `$CHASE_INSTALL_DIR/chase/bin` directory.

## dsl_tool

The tool is provided by the [DSL_tool][DSL_TOOL] module. It takes as input a
problem specification expressed using the CHASE Domain Specific Language and
encodes it for a GR(1) synthesis tool ([slugs][SLUGS] or [gr1c][GR1C]), which
is then used to check the realizability of the specification and to synthesize
a controller.

The CHASE specification language accepted by the tool allows specifying a set
of requirements about the system behavior over an architecture represented by
a network.

>**Usage**
>
> `dsl_tool -i input_file.txt [-o output_file] [-b backend] [-V]`

> **Parameters:**
>
> - -i _Specifies the `.txt` input file containing the specification._
> - -o _Specifies the output file._
> - -b _Specifies the backend to be used. It may either be: `slugs` (default value) or `gr1c`._
> - -V _Enables verbose mode._

For instance, from the root of the [demos][DEMOS] repository:

`$> dsl_tool -i DATE18/template_0_realizable.txt -o /tmp/template_0 -V`

It will generate the _structured slugs_ file `/tmp/template_0.structuredSlugs`.
Meanwhile, the following command:

`$> dsl_tool -i DATE18/template_0_realizable.txt -o /tmp/template_0 -b gr1c`

will generate the _GR1C_ specification `/tmp/template_0.spc`, whose
realizability can be checked with `gr1c -r /tmp/template_0.spc`.

## logics_tool

The tool is provided by the [logics_tool][LOGICS_TOOL] module. It parses a set
of assume-guarantee contracts expressed using the logics-based language of
CHASE, and opens a console to manipulate them.

>**Usage**
>
> `logics_tool -i input_file [-o output_dir] [-c commands_file] [-V]`

> **Parameters:**
>
> - -i _Specifies the input file containing the contracts._
> - -o _Specifies the directory where the generated files are written._
> - -c _Specifies a file of console commands (each terminated by `;`) to be
>   executed instead of opening the interactive console._
> - -V _Enables verbose mode._

The console commands include `show`, `compose`, `conjunction`,
`disjunction`, `merging`, `quotient`, `separation`, `saturate`, `simplify`,
`refinement`, `verify`, and `synthesize`; type `help` in the console for their
description. The [DATE21 demo][DATE21] shows a complete session.

For the usage of [slugs][SLUGS], [GR1C][GR1C], and [nuXmv][NUXMV], we refer the
reader to the tools documentation.

[<-- Go back to the main page][HOME]

[SLUGS]: https://github.com/VerifiableRobotics/slugs
[GR1C]: https://github.com/tulip-control/gr1c
[NUXMV]: https://nuxmv.fbk.eu/
[DSL_TOOL]: https://github.com/chase-cps/DSL_tool
[LOGICS_TOOL]: https://github.com/chase-cps/logics_tool
[DEMOS]: https://github.com/chase-cps/demos
[DATE21]: https://github.com/chase-cps/demos/blob/main/DATE21/DATE21.md
[HOME]: ../../README.md
