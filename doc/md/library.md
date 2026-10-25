# CHASE representation library

The CHASE representation library is meant to provide a unified representation
for systems specifications based on a set of heterogeneous formal representations.
It aims at providing a complete API for the representation and manipulation
of such specifications, with the purpose of enhancing automation of formal
reasoning in system design.

The library is released with CHASE, and all the tools are built on top of such
library. The detailed documentation of the API is provided by relying on Doxygen.
The CMake of the project specifies the ``doc`` target which allows to generate
the Doxygen documentation for the library. I.e., to generate the documentation,
the user just need to type the following command from the build directory:

``$> cmake ..``

``$> make doc``

The last command assumes Doxygen to be installed in the machine;
it guarantees the generation of the classic Doxygen format documentation.

## Abstract Syntax Tree

Any representation in CHASE is based on a Abstract Syntax Tree (AST).
Every node of the tree derives from `chase::ChaseObject`
(`include/representation/ChaseObject.hh`), which stores the type of the node
(see the `nodeType` enumeration) and provides the `accept_visitor` method used
by the visitors.

The main groups of nodes are:

- **Systems and problems**: `System`, `DesignProblem`, `Component`,
  `ComponentDefinition`, `Library`.
- **Specifications**: `Specification` and `Contract`, the latter storing
  assumptions and guarantees and implementing the contract algebra
  (`include/representation/Contract.hh`).
- **Declarations and types**: `Variable`, `Constant`, `Parameter`, `Function`,
  and the types `Boolean`, `Integer`, `Real`, `String`, `Enumeration`, ...
- **Expressions and values**: `Expression`, `Identifier`, `IntegerValue`,
  `RealValue`, `Range`, `Interval`, `Matrix`, ...
- **Logic formulas**: `Proposition`, `UnaryBooleanFormula`,
  `BinaryBooleanFormula`, `LargeBooleanFormula`, `UnaryTemporalFormula`,
  `BinaryTemporalFormula`, `ModalFormula`, `QuantifiedFormula`.

Formulas are usually built through the helper functions declared in
`include/utilities/Factory.hh` (e.g., `And`, `Or`, `Not`, `Implies`, `Always`,
`Eventually`), rather than by calling the constructors directly.

### Extending the library

Indeed, the library is meant to be extended in order to support a larger number
of formal representations. The AST is traversed by means of the visitor
pattern:

- `chase::BaseVisitor` (`include/utilities/BaseVisitor.hh`) declares one
  `visit` method per node type;
- `chase::GuideVisitor` (`include/utilities/GuideVisitor.hh`) implements a
  default traversal of the whole tree, so that a new visitor only needs to
  override the methods for the nodes it is interested in.

The visitors in `include/utilities/` (e.g., `LogicSimplificationVisitor`,
`ClonedDeclarationVisitor`) and the printers of the `chase-backends` module
are examples of this approach. To add a new kind of node, derive it from
`ChaseObject`, add the corresponding value to `nodeType`, and add the
corresponding `visit` method to `BaseVisitor` and `GuideVisitor`.
