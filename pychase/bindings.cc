#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "chase-core.hh"

namespace py = pybind11;
using namespace chase;

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

PYBIND11_MODULE(pychase, k) {
    k.doc() = "CHASE Core  Python wrapper module";

    auto m = k.def_submodule("representation", 
        "Wrapper for the Chase core representation library.");

    py::enum_<chase::nodeType>(m, "nodeType")
        .value("object_node", chase::object_node)
        .value("system_node", chase::system_node)
        .value("design_problem_node", chase::design_problem_node)
        .value("specification_node", chase::specification_node)
        .value("contract_node", chase::contract_node)
        .value("boolean_node", chase::boolean_node)
        .value("booleanValue_node", chase::booleanValue_node)
        .value("constant_node", chase::constant_node)
        .value("dataDeclaration_node", chase::dataDeclaration_node)
        .value("declaration_node", chase::declaration_node)
        .value("componentDefinition_node", chase::componentDefinition_node)
        .value("component_node", chase::component_node)
        .value("expression_node", chase::expression_node)
        .value("identifier_node", chase::identifier_node)
        .value("integer_node", chase::integer_node)
        .value("integerValue_node", chase::integerValue_node)
        .value("interval_node", chase::interval_node)
        .value("name_node", chase::name_node)
        .value("numericValue_node", chase::numericValue_node)
        .value("range_node", chase::range_node)
        .value("real_node", chase::real_node)
        .value("realValue_node", chase::realValue_node)
        .value("string_node", chase::string_node)
        .value("stringValue_node", chase::stringValue_node)
        .value("simpleType_node", chase::simpleType_node)
        .value("customType_node", chase::customType_node)
        .value("enumeration_node", chase::enumeration_node)
        .value("type_node", chase::type_node)
        .value("value_node", chase::value_node)
        .value("matrix_node", chase::matrix_node)
        .value("variable_node", chase::variable_node)
        .value("parameter_node", chase::parameter_node)
        .value("distribution_node", chase::distribution_node)
        .value("proposition_node", chase::proposition_node)
        .value("unaryBooleanOperation_node", chase::unaryBooleanOperation_node)
        .value("binaryBooleanOperation_node", chase::binaryBooleanOperation_node)
        .value("booleanConstant_node", chase::booleanConstant_node)
        .value("modalFormula_node", chase::modalFormula_node)
        .value("largeBooleanFormula_node", chase::largeBooleanFormula_node)
        .value("unaryTemporalOperation_node", chase::unaryTemporalOperation_node)
        .value("binaryTemporalOperation_node", chase::binaryTemporalOperation_node)
        .value("quantified_formula_node", chase::quantifiedFormula_node)
        .value("graphEdge_node", chase::graphEdge_node)
        .value("graphVertex_node", chase::graphVertex_node)
        .value("graph_node", chase::graph_node)
        .value("library_node", chase::library_node)
        .value("functionCall_node", chase::functionCall_node)
        .value("probabilityFunction_node", chase::probabilityFunction_node)
        .value("relation_node", chase::relation_node)
        .value("constraint_node", chase::constraint_node)
        .export_values();

    py::class_<ChaseObject, std::unique_ptr<ChaseObject, 
        py::nodelete>>
        (m, "ChaseObject")
        .def("getParent", &ChaseObject::getParent)
        .def("setParent", &ChaseObject::setParent,
            py::arg("parent").none(false))
        .def("accept_visitor", &ChaseObject::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &ChaseObject::getString)
        .def("clone", &ChaseObject::clone)
        .def("IsA", &ChaseObject::IsA);

    py::class_<DesignProblem, std::unique_ptr<DesignProblem, py::nodelete>,
        ChaseObject>(m, "DesignProblem")
        .def(py::init<>())
        .def("getSystem", &DesignProblem::getSystem)
        .def("setSystem", &DesignProblem::setSystem,
            py::arg("system").none(false))
        .def("accept_visitor", &DesignProblem::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &DesignProblem::getString)
        .def("clone", &DesignProblem::clone);

    py::class_<Name, std::unique_ptr<Name, 
        py::nodelete>, ChaseObject>
        (m, "Name")
        .def(py::init<std::string &>(), py::arg("s"))
        .def(py::init<const Name &>(), py::arg("o"))
        .def("accept_visitor", &Name::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Name::getString)
        .def("changeName", &Name::changeName, 
            py::arg("name"))
        .def("clone", &Name::clone);

    /**
    *  OPERATORS 
    */
    
    py::enum_<chase::Operator>(m, "Operator")
        .value("op_none", chase::Operator::op_none)
        .value("op_plus", chase::Operator::op_plus)
        .value("op_minus", chase::Operator::op_minus)
        .value("op_multiply", chase::Operator::op_multiply)
        .value("op_divide", chase::Operator::op_divide)
        .value("op_mod", chase::Operator::op_mod)
        .value("op_eq", chase::Operator::op_eq)
        .value("op_neq", chase::Operator::op_neq)
        .value("op_lt", chase::Operator::op_lt)
        .value("op_gt", chase::Operator::op_gt)
        .value("op_le", chase::Operator::op_le)
        .value("op_ge", chase::Operator::op_ge)
        .export_values();

    py::enum_<chase::BooleanOperator>(m, "BooleanOperator")
        .value("op_not", chase::BooleanOperator::op_not)
        .value("op_and", chase::BooleanOperator::op_and)
        .value("op_or", chase::BooleanOperator::op_or)
        .value("op_implies", chase::BooleanOperator::op_implies)
        .value("op_iff", chase::BooleanOperator::op_iff)
        .value("op_xor", chase::BooleanOperator::op_xor)
        .value("op_nand", chase::BooleanOperator::op_nand)
        .value("op_nor", chase::BooleanOperator::op_nor)
        .value("op_xnor", chase::BooleanOperator::op_xnor)
        .export_values();
    
    py::enum_<chase::ModalOperator>(m, "ModalOperator")
        .value("op_square", chase::ModalOperator::op_square)
        .value("op_diamond", chase::ModalOperator::op_diamond)
        .export_values();

    py::enum_<chase::TemporalOperator>(m, "TemporalOperator")
        .value("op_globally", chase::TemporalOperator::op_globally)
        .value("op_future", chase::TemporalOperator::op_future)
        .value("op_next", chase::TemporalOperator::op_next)
        .value("op_until", chase::TemporalOperator::op_until)
        .value("op_release", chase::TemporalOperator::op_release)
        .export_values();


    /**
    *   TYPES BINDINGS
    */

    // Type binding.
    py::class_<Type, std::unique_ptr<Type, 
        py::nodelete>, ChaseObject>(m, "Type")
        .def("getTypeVariant", &Type::getTypeVariant)
        .def("setTypeVariant", &Type::setTypeVariant,
            py::arg("tv").none(false))
        .def("getTypeVariantString", &Type::getTypeVariantString)
        .def("accept_visitor", &Type::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Type::getString)
        .def("clone", &Type::clone);

    py::class_<SimpleType, Type, std::unique_ptr<SimpleType, 
        py::nodelete>>(m, "SimpleType");

    // CustomType binding.
    py::class_<CustomType, Type, std::unique_ptr<CustomType,
        py::nodelete>>(m, "CustomType")
        .def(py::init<std::string, Type *>(),
            py::arg("name")=std::string(""),
            py::arg("type")=nullptr)
        .def(py::init<Name*, Type *>(),
            py::arg("name")=new Name(""),
            py::arg("type")=nullptr)
        .def("getType", &CustomType::getType)
        .def("setType", &CustomType::setType,
            py::arg("type").none(false))
        .def("getName", &CustomType::getName)
        .def("setName", &CustomType::setName,
            py::arg("name").none(false))
        .def("accept_visitor", &CustomType::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &CustomType::getString)
        .def("clone", &CustomType::clone);

    // Enumeration binding.
    py::class_<Enumeration, CustomType, 
        std::unique_ptr<Enumeration, py::nodelete>>(m, "Enumeration")
        .def(py::init<std::string>(),
            py::arg("name")="")
        .def(py::init<Name *>(),
            py::arg("name").none(false))
        .def("addItem", &Enumeration::addItem,
            py::arg("item").none(false))
        .def("getItemInPosition", &Enumeration::getItemInPosition,
            py::arg("position").none(false))
        .def("getPositionByName", &Enumeration::getPositionByName,
            py::arg("name").none(false))
        .def("accept_visitor", &Enumeration::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Enumeration::getString)
        .def("clone", &Enumeration::clone);

    // Boolean Type
    py::class_<Boolean, std::unique_ptr<Boolean, py::nodelete>, SimpleType>
        (m, "Boolean")
        .def(py::init<>())
        .def("accept_visitor", &Boolean::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Boolean::getString)
        .def("clone", &Boolean::clone);

    // Integer Type
    py::class_<Integer, std::unique_ptr<Integer, 
        py::nodelete>, 
        SimpleType>(m, "Integer")
        .def(py::init<>())
        .def(py::init<const int64_t &, const int64_t &>(),
            py::arg("min").none(false), py::arg("max").none(false))
        .def("isSigned", &Integer::isSigned)
        .def("getMin", &Integer::getMin)
        .def("getMax", &Integer::getMax)
        .def("accept_visitor", &Integer::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Integer::getString)
        .def("clone", &Integer::clone);

    // Real Type
    py::class_<Real, std::unique_ptr<Real, py::nodelete>,
        SimpleType>(m, "Real")
        .def(py::init<>())
        .def(py::init<const double &, const double &>(),
             py::arg("min").none(false), py::arg("max").none(false))
        .def("getMin", &Real::getMin)
        .def("getMax", &Real::getMax)
        .def("accept_visitor", &Real::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Real::getString)
        .def("clone", &Real::clone);



    /**
    *   VALUES BINDINGS
    */

 // Value binding.
    py::class_<Value, std::unique_ptr<Value, py::nodelete>, 
        ChaseObject>(m, "Value")
        .def("getType", &Value::getType)
        .def("setType", &Value::setType,
            py::arg("type").none(false))
        .def("clone", &Value::clone);

    // NumericValue binding.
    py::class_<NumericValue, std::unique_ptr<NumericValue, 
        py::nodelete>, Value>(m, "NumericValue");


    //  Identifier Binding
    py::class_<Identifier, std::unique_ptr<Identifier, 
        py::nodelete>, Value>(m, "Identifier")
        .def(py::init<DataDeclaration *>(),
            py::arg("d")=nullptr)
        .def(py::init<const Identifier &>(), py::arg("i"))
        .def("getDeclaration", &Identifier::getDeclaration)
        .def("setDeclaration", &Identifier::setDeclaration, 
            py::arg("d"))
        .def("getString", &Identifier::getString)
        .def("getType", &Identifier::getType)
        .def("clone", &Identifier::clone);

    // Expression Binding
    py::class_<Expression, std::unique_ptr<Expression, 
        py::nodelete>, Value>(m, "Expression")
        .def(py::init<>())
        .def(py::init<Operator, Value *,
            Value *>(), py::arg("op").none(false),
            py::arg("op1").none(false), py::arg("op2")=nullptr)
        .def("getOperator", &Expression::getOperator)
        .def("getOp1", &Expression::getOp1)
        .def("getOp2", &Expression::getOp2)
        .def("setOperator", &Expression::setOperator, 
            py::arg("op").none(false))
        .def("setOp1", &Expression::setOp1,
            py::arg("op").none(false))
        .def("setOp2", &Expression::setOp2,
            py::arg("op").none(false))
        .def("getString", &Expression::getString)
        .def("getType", &Expression::getType)
        .def("accept_visitor", &Expression::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Expression::clone);


    //  BooleanValue Binding
    py::class_<BooleanValue, std::unique_ptr<BooleanValue, 
        py::nodelete>, NumericValue>(m, "BooleanValue")
        .def(py::init<const bool &>(),
            py::arg("value") = false)
        .def(py::init<const BooleanValue &>(),
            py::arg("o").none(false))
        .def("getValue", &BooleanValue::getValue)
        .def("setValue", &BooleanValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &BooleanValue::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BooleanValue::getString)
        .def("clone", &BooleanValue::clone);

    //  IntegerValue Binding
    py::class_<IntegerValue, std::unique_ptr<IntegerValue, 
        py::nodelete>, NumericValue>(m, "IntegerValue")
        .def(py::init<const int64_t &>(),
            py::arg("value") = 0)
        .def(py::init<const IntegerValue &>(),
            py::arg("o").none(false))
        .def("getValue", &IntegerValue::getValue)
        .def("setValue", &IntegerValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &IntegerValue::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &IntegerValue::getString)
        .def("clone", &IntegerValue::clone);

    //  RealValue Binding
    py::class_<RealValue, std::unique_ptr<RealValue, 
        py::nodelete>, NumericValue>(m, "RealValue")
        .def(py::init<const double &>(),
            py::arg("value") = 0)
        .def(py::init<const RealValue &>(),
            py::arg("o").none(false))
        .def("getValue", &RealValue::getValue)
        .def("setValue", &RealValue::setValue,
            py::arg("v").none(false))
        .def("accept_visitor", &RealValue::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &RealValue::getString)
        .def("clone", &RealValue::clone);

    // Range Binding
    py::class_<Range, std::unique_ptr<Range,
    py::nodelete>, Value>(m, "Range")
    .def(py::init<>())
    .def(py::init<const int &, const int &>(),
         py::arg("lbound").none(false),
         py::arg("rbound").none(false))
    .def("getType", &Range::getType,
        py::return_value_policy::reference)
    .def("setRightValue", &Range::setRightValue,
       py::arg("rbound").none(false))
   .def("setLeftValue", &Range::setLeftValue,
            py::arg("lbound").none(false))
    .def("getLeftValue", &Range::getLeftValue)
    .def("getRightValue", &Range::getRightValue)
    .def("getString", &Range::getString)
    .def("accept_visitor", &Range::accept_visitor,
         py::arg("v").none(false))
     .def("clone", &Range::clone);

    // Interval Binding
    py::class_<Interval, std::unique_ptr<Interval,
    py::nodelete>, Value>(m, "Interval")
    .def(py::init<Value *, Value*, bool, bool>(),
            py::arg("lbound").none(false),
            py::arg("rbound").none(false),
            py::arg("leftOpen").none(false),
            py::arg("rightOpen").none(false))
    .def("getLeftBound", &Interval::getLeftBound,
         py::return_value_policy::reference)
    .def("getRightBound", &Interval::getRightBound,
         py::return_value_policy::reference)
    .def("isLeftOpen", &Interval::isLeftOpen)
    .def("isRightOpen", &Interval::isRightOpen)
    .def("getType", &Interval::getType)
    .def("getString", &Interval::getString)
    .def("accept_visitor", &Interval::accept_visitor,
         py::arg("v").none(false))
    .def("clone", &Interval::clone);

    // Matrix Binding.
    py::class_<Matrix, std::unique_ptr<Matrix, 
        py::nodelete>, Value>(m, "Matrix")
        .def(py::init<unsigned int, unsigned int, std::vector<Value*> >(),
            py::arg("rows").none(false),
            py::arg("columns").none(false),
            py::arg("M").none(true))    
        .def("getRows", &Matrix::getRows, py::return_value_policy::copy)
        .def("getColumns", &Matrix::getColumns, py::return_value_policy::copy)
        .def("at", &Matrix::at, py::return_value_policy::reference,
            py::arg("i").none(false),
            py::arg("j").none(false),
            py::arg("value").none(true))
        .def("getString", &Matrix::getString)
        .def("accept_visitor", &Matrix::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Matrix::clone);
    
    // FunctionCall Binding.
    py::class_<FunctionCall, Value, std::unique_ptr<FunctionCall,
        py::nodelete>>(m, "FunctionCall")
        .def(py::init<>())
        .def("getFunction", &FunctionCall::getFunction)
        .def("setFunction", &FunctionCall::setFunction,
            py::arg("function").none(false),
            py::arg("initialize")=true)
        .def("parameter", &FunctionCall::parameter,
            py::arg("i").none(false),
            py::arg("value")=nullptr)
        .def("getString", &FunctionCall::getString)
        .def("accept_visitor", &FunctionCall::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &FunctionCall::clone);

    // Probability Function.
    py::class_<ProbabilityFunction, Value, std::unique_ptr<
        ProbabilityFunction, py::nodelete>>(m, "ProbabilityFunction")
        .def(py::init<Specification *>(),
            py::arg("specification")=nullptr)
        .def("getSpecification", &ProbabilityFunction::getSpecification)
        .def("setSpecification", &ProbabilityFunction::setSpecification,
            py::arg("specification").none(false))
        .def("getString", &ProbabilityFunction::getString)
        .def("accept_visitor", &ProbabilityFunction::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &ProbabilityFunction::clone);

    /**
    *   DECLARATION BINDINGS
    */

    // Declaration binding.
    py::class_<Declaration, std::unique_ptr<Declaration, 
        py::nodelete>, ChaseObject>(m, "Declaration")
        .def("getName", &Declaration::getName)
        .def("setName", &Declaration::setName,
            py::arg("name").none(false))
        ;

    // Scope binding.
    py::class_<Scope, std::unique_ptr<Scope, py::nodelete>, 
        Declaration>(m, "Scope")
        .def("addDeclaration", &Scope::addDeclaration,
            py::arg("declaration").none(false))
        .def_readwrite("declarations", &Scope::declarations)
        ;

    // Library binding.
    py::class_<Library, Scope, std::unique_ptr<Library, py::nodelete>>
        (m, "Library")
        .def(py::init<>())
        .def(py::init<std::string>(),
            py::arg("name").none(false))
        .def(py::init<Name*>(),
            py::arg("name").none(false))
        .def("accept_visitor", &Library::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Library::getString)
        .def("clone", &Library::clone);

    // ComponentDefinition Binding
    py::class_<ComponentDefinition, std::unique_ptr<
        ComponentDefinition, py::nodelete>, 
        Declaration>(m, "ComponentDefinition")
        .def(py::init<>())
        .def(py::init<Name *>(),
            py::arg("name").none(false))
        .def(py::init<std::string &>(),
            py::arg("name").none(false))
        .def("accept_visitor", &ComponentDefinition::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &ComponentDefinition::getString)
        .def("clone", &ComponentDefinition::clone)
        .def_readwrite("views", &ComponentDefinition::views)
        .def_readwrite("subcomponents", &ComponentDefinition::subcomponents);
    
    // DataDeclaration Binding
    py::class_<DataDeclaration, std::unique_ptr<DataDeclaration, 
        py::nodelete>, Declaration>(m, "DataDeclaration")
        .def("getType", &DataDeclaration::getType)
        .def("setType", &DataDeclaration::setType,
            py::arg("t").none(false));
    
    //Parameter Binding
    py::class_<Parameter, std::unique_ptr<Parameter, 
        py::nodelete>, DataDeclaration>(m, "Parameter")
        .def(py::init<Type *, Name *>(),
            py::arg("type")=nullptr, 
            py::arg("name")=nullptr)
        .def("accept_visitor", &Parameter::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Parameter::getString)
        .def("clone", &Parameter::clone);

    // Constant Binding
    py::class_<Constant, std::unique_ptr<Constant, 
        py::nodelete>, DataDeclaration>(m, "Constant")
        .def(py::init< Type *, Name *, Value* >(), 
            py::arg("type"), 
            py::arg("name"), 
            py::arg("value"))
        .def("getString", &Constant::getString)
        .def("getValue", &Constant::getValue)
        .def("setValue", &Constant::setValue,
            py::arg("value").none(false))
        .def("accept_visitor", &Constant::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Constant::clone);

    // Causality Enum
    py::enum_<chase::causality_t>(m, "causality_t")
        .value("generic", chase::causality_t::generic)
        .value("input", chase::causality_t::input)
        .value("output", chase::causality_t::output)
        .value("inout", chase::causality_t::inout)
        .value("internal", chase::causality_t::internal)
        .export_values();
        
    // Variable Binding
    py::class_<Variable, std::unique_ptr<Variable, 
        py::nodelete>, DataDeclaration>(m, "Variable")
        .def(py::init<Type *, Name *, causality_t &>(),
            py::arg("type"), 
            py::arg("name"), 
            py::arg("causality_t")=generic)
        .def("getString", &Variable::getString)
        .def("getCausality", &Variable::getCausality)
        .def("setCausality", &Variable::setCausality, 
            py::arg("causality").none(false))
        .def("accept_visitor", &Variable::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &Variable::clone);

    // Distribution type enumeration binding.
    py::enum_<distribution_type>(m, "distribution_type", py::arithmetic())
        .value("custom", distribution_type::custom)
        .value("gaussian", distribution_type::gaussian)
        .value("homogeneous", distribution_type::homogeneous)
        .export_values();

    // Distribution Binding.
    py::class_<Distribution, std::unique_ptr<Distribution, 
        py::nodelete>, DataDeclaration>(m, "Distribution")
        .def_readwrite("parameters", &Distribution::parameters)
        .def(py::init<distribution_type &, Name * , Type *>(),
            py::arg("dtype").none(false),
            py::arg("name").none(true),
            py::arg("type").none(true))
        .def("getDistributionType", &Distribution::getDistributionType)
        .def("setDistributionType", &Distribution::setDistributionType,
            py::arg("distributionType").none(false))
        .def("parameter", &Distribution::parameter,
            py::arg("name").none(false),
            py::arg("value").none(true))
        .def("accept_visitor", &Distribution::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Distribution::getString)
        .def("clone", &Distribution::clone);

    // Function Binding.
    py::class_<Function, std::unique_ptr<Function, py::nodelete>,
        DataDeclaration>(m, "Function")
        .def_readwrite("parameters", &Function::parameters)
        .def(py::init<Type *, Name *, unsigned int>(),
            py::arg("type").none(false),
            py::arg("name").none(false),
            py::arg("arity").none(false))
        .def("getArity", &Function::getArity)
        .def("setArity", &Function::setArity,
            py::arg("arity").none(false))
        .def("getDomainOfParameter", &Function::getDomainOfParameter)
        .def("setDomainOfParameter", &Function::setDomainOfParameter,
            py::arg("position").none(false),
            py::arg("type").none(false))
        .def("accept_visitor", &Function::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Function::getString)
        .def("clone", &Function::clone);
        
    /**
    *  SPECIFICATIONS BINDINGS
    */

    py::class_<Specification, std::unique_ptr<Specification, 
        py::nodelete>, ChaseObject>(m, "Specification");
    py::class_<LogicFormula, std::unique_ptr<LogicFormula, 
        py::nodelete>, Specification>(m, "LogicFormula");

    py::enum_<chase::semantic_domain>(m, "semantic_domain")
        .value("logic", chase::semantic_domain::logic)
        .value("graph", chase::semantic_domain::graph)
        .export_values();

        py::enum_<chase::logic_quantifier>(m, "logic_quantifier")
        .value("forall", logic_quantifier::forall)
        .value("exists", logic_quantifier::exists)
        .export_values();

    // Constraint binding.
    py::class_<Constraint, std::unique_ptr<Constraint, py::nodelete>,
        Specification>(m, "Constraint")
        .def(py::init<>())
        .def("getExpression", &Constraint::getExpression)
        .def("setExpression", &Constraint::setExpression,
            py::arg("expression").none(false))
        .def("accept_visitor", &Constraint::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Constraint::getString)
        .def("clone", &Constraint::clone);

    // BinaryBooleanFormula
    py::class_<BinaryBooleanFormula, 
        std::unique_ptr<BinaryBooleanFormula, py::nodelete>, 
        LogicFormula>(m, "BinaryBooleanFormula")
        .def(py::init<chase::BooleanOperator, 
            LogicFormula *, 
            LogicFormula *>(),
            py::arg("op")=op_and, 
            py::arg("op1")=nullptr,
            py::arg("op2")=nullptr)
        .def("getOp", &BinaryBooleanFormula::getOp)
        .def("getOp1", &BinaryBooleanFormula::getOp1)
        .def("getOp2", &BinaryBooleanFormula::getOp2)
        .def("setOp", &BinaryBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("setOp1", &BinaryBooleanFormula::setOp1,
            py::arg("op1").none(false))
        .def("setOp2", &BinaryBooleanFormula::setOp2,
            py::arg("op2").none(false))
        .def("accept_visitor", &BinaryBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BinaryBooleanFormula::getString)
        .def("clone", &BinaryBooleanFormula::clone);
    
    // BinaryTemporalFormula
    py::class_<BinaryTemporalFormula, 
        std::unique_ptr<BinaryTemporalFormula, py::nodelete>, 
        LogicFormula>(m, "BinaryTemporalFormula")
        .def(py::init<chase::TemporalOperator, 
            LogicFormula *, 
            LogicFormula *>(),
            py::arg("op")=op_until, 
            py::arg("op1")=nullptr,
            py::arg("op2")=nullptr)
        .def("getOp", &BinaryTemporalFormula::getOp)
        .def("getFormula1", &BinaryTemporalFormula::getFormula1)
        .def("getFormula2", &BinaryTemporalFormula::getFormula2)
        .def("setOp", &BinaryTemporalFormula::setOp,
            py::arg("op").none(false))
        .def("setFormula1", &BinaryTemporalFormula::setFormula1,
            py::arg("formula1").none(false))
        .def("setFormula2", &BinaryTemporalFormula::setFormula2,
            py::arg("formula2").none(false))
        .def("getString", &BinaryTemporalFormula::getString)
        .def("accept_visitor", &BinaryTemporalFormula::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &BinaryTemporalFormula::clone);
    
    // Boolean Constant
    py::class_<BooleanConstant,
        std::unique_ptr<BooleanConstant, py::nodelete>, 
        LogicFormula>(m, "BooleanConstant")
        .def(py::init<bool &>(), py::arg("value")=true)
        .def("getValue", &BooleanConstant::getValue)
        .def("accept_visitor", &BooleanConstant::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &BooleanConstant::getString)
        .def("clone", &BooleanConstant::clone);
    
    // LargeBooleanFormula
    py::class_<LargeBooleanFormula,
        std::unique_ptr<LargeBooleanFormula, py::nodelete>,
        LogicFormula>(m, "LargeBooleanFormula")
        .def(py::init<BooleanOperator &>(),
            py::arg("op")=op_and)
        .def("getOp", &LargeBooleanFormula::getOp)
        .def("setOp", &LargeBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("addOperand", &LargeBooleanFormula::addOperand,
            py::arg("f").none(false))
        .def("getString", &LargeBooleanFormula::getString)
        .def("accept_visitor", &LargeBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("clone", &LargeBooleanFormula::clone)
        .def_readwrite("operands", &LargeBooleanFormula::operands);

    // ModalFormula
    py::class_<ModalFormula,
        std::unique_ptr<ModalFormula, py::nodelete>,
        LogicFormula>(m, "ModalFormula")
        .def(py::init<ModalOperator &,
            LogicFormula *>(),
            py::arg("op")=op_square, 
            py::arg("formula")=nullptr)
        .def("getOperator", &ModalFormula::getOperator)
        .def("getFormula", &ModalFormula::getFormula)
        .def("setOperator", &ModalFormula::setOperator,
            py::arg("op").none(false))
        .def("setFormula", &ModalFormula::setFormula,
            py::arg("formula").none(false))
        .def("getString", &ModalFormula::getString)
        .def("clone", &ModalFormula::clone);

    // Proposition
    py::class_<Proposition, 
        std::unique_ptr<Proposition, py::nodelete>,
        LogicFormula>(m, "Proposition")
        .def(py::init<>())
        .def(py::init<Value *>(),
            py::arg("v").none(false))
        .def("getType", &Proposition::getType)
        .def("getValue", &Proposition::getValue)
        .def("getName", &Proposition::getName)
        .def("setValue", &Proposition::setValue,
            py::arg("v").none(false))
        .def("setName", &Proposition::setName,
            py::arg("n").none(false))
        .def("accept_visitor", &Proposition::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Proposition::getString)
        .def("clone", &Proposition::clone);

    // UnaryBooleanFormula
    py::class_<UnaryBooleanFormula,
        std::unique_ptr<UnaryBooleanFormula, py::nodelete>,
        LogicFormula>(m, "UnaryBooleanFormula")
        .def(py::init<BooleanOperator &,
            LogicFormula *>(),
            py::arg("op")=op_not,
            py::arg("op1")=nullptr)
        .def("getOp", &UnaryBooleanFormula::getOp)
        .def("getOp1", &UnaryBooleanFormula::getOp1)
        .def("setOp", &UnaryBooleanFormula::setOp,
            py::arg("op").none(false))
        .def("setOp1", &UnaryBooleanFormula::setOp1,
            py::arg("op1").none(false))
        .def("accept_visitor", &UnaryBooleanFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &UnaryBooleanFormula::getString)
        .def("clone", &UnaryBooleanFormula::clone);

    // UnaryTemporalFormula
    py::class_<UnaryTemporalFormula,
        std::unique_ptr<UnaryTemporalFormula, py::nodelete>,
        LogicFormula>(m, "UnaryTemporalFormula")
        .def(py::init<TemporalOperator &,
            LogicFormula *>(),
            py::arg("op")=op_globally,
            py::arg("op1")=nullptr)
        .def("getOp", &UnaryTemporalFormula::getOp)
        .def("getFormula", &UnaryTemporalFormula::getFormula)
        .def("setOp", &UnaryTemporalFormula::setOp,
            py::arg("op").none(false))
        .def("setFormula", &UnaryTemporalFormula::setFormula,
            py::arg("formula").none(false))
        .def("accept_visitor", &UnaryTemporalFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &UnaryTemporalFormula::getString)
        .def("clone", &UnaryTemporalFormula::clone);



    // QuantifiedFormula binding.
    py::class_<QuantifiedFormula, std::unique_ptr<QuantifiedFormula,
        py::nodelete>, LogicFormula>(m, "QuantifiedFormula")
        .def(py::init<logic_quantifier, Variable *, LogicFormula *>(),
            py::arg("quantifier").none(false),
            py::arg("variable").none(false),
            py::arg("formula").none(false))
        .def("getQuantifier", &QuantifiedFormula::getQuantifier)
        .def("getVariable", &QuantifiedFormula::getVariable)
        .def("getFormula", &QuantifiedFormula::getFormula)
        .def("setQuantifier", &QuantifiedFormula::setQuantifier,
            py::arg("quantifier").none(false))
        .def("setVariable", &QuantifiedFormula::setVariable,
            py::arg("variable").none(false))
        .def("setFormula", &QuantifiedFormula::setFormula,
            py::arg("formula").none(false))
        .def("accept_visitor", &QuantifiedFormula::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &QuantifiedFormula::getString)
        .def("clone", &QuantifiedFormula::clone);
    
    /**
    * GRAPH BINDINGS
    */ 

    // Edge  
    py::class_<Edge, std::unique_ptr<Edge, py::nodelete>, 
        ChaseObject>(m, "Edge")
        .def(py::init<unsigned int &, unsigned int &>(),
            py::arg("source").none(false), 
            py::arg("target").none(false))
        .def("getSource", &Edge::getSource)
        .def("setSource", &Edge::setSource,
            py::arg("source").none(false))
        .def("getTarget", &Edge::getTarget)
        .def("setTarget", &Edge::setTarget,
            py::arg("target").none(false))
        .def("accept_visitor", &Edge::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Edge::getString)
        .def("clone", &Edge::clone);

    // WeightedEdge
    py::class_<WeightedEdge, 
        std::unique_ptr<WeightedEdge, py::nodelete>, Edge>
        (m, "WeightedEdge")
        .def(py::init<unsigned int &, 
            unsigned int &,
            Value *>(),
            py::arg("source").none(false), 
            py::arg("target").none(false),
            py::arg("weight").none(false))
        .def("getWeight", &WeightedEdge::getWeight)
        .def("setWeight", &WeightedEdge::setWeight,
            py::arg("weight").none(false))
        .def("getString", &Edge::getString)
        .def("clone", &Edge::clone);
    
    // Vertex
    py::class_<Vertex, std::unique_ptr<Vertex, py::nodelete>,
        ChaseObject>(m, "Vertex")
        .def(py::init<Name *>(),
            py::arg("name")=nullptr)
        .def("getName", &Vertex::getName)
        .def("setName", &Vertex::setName,
            py::arg("name").none(false))
        .def("accept_visitor", &Vertex::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Vertex::getString)
        .def("clone", &Vertex::clone);
    
    // Graph
    py::class_<Graph, std::unique_ptr<Graph, py::nodelete>,
        Specification>(m, "Graph")
        .def(py::init<unsigned int &, 
            bool &, Name *>(),
            py::arg("size").none(false), 
            py::arg("directed")=false,
            py::arg("name")=new Name("GenericGraph"))
        .def("accept_visitor", &Graph::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Graph::getString)
        .def("associateVertex", &Graph::associateVertex,
            py::arg("index").none(false),
            py::arg("vertex").none(false))
        .def("addEdge", &Graph::addEdge,
            py::arg("edge").none(false))
        .def("isDirected", &Graph::isDirected)
        .def("getEdge", &Graph::getEdge,
            py::arg("source").none(false),
            py::arg("target").none(false))
        .def("getVertex", &Graph::getVertex,
            py::arg("vertex_id").none(false))
        .def("getSize", &Graph::getSize)
        .def("getVertexIndex", &Graph::getVertexIndex,
            py::arg("name").none(false))
        .def("getName", &Graph::getName)
        .def("setName", &Graph::setName,
            py::arg("name"))
        .def("getGraphViz", &Graph::getGraphViz)
        .def("getAdjacentNodes", &Graph::getAdjacentNodes,
            py::arg("id").none(false))
        .def("clone", &Graph::clone);

    m.def("getSubGraph", &chase::getSubGraph,
        py::arg("graph"), py::arg("vertexes"));
    m.def("findAllPathsBetweenNodes", &chase::findAllPathsBetweenNodes,
        py::arg("graph"), py::arg("visited"),
        py::arg("end"), py::arg("result"));

    
    /**
    * SYSTEM BINDINGS
    */ 
    py::class_<System, std::unique_ptr<System,
        py::nodelete>, ChaseObject>(m, "System")
        .def(py::init<std::string &>(),
            py::arg("name")="System")
        .def("addDeclaration", &System::addDeclaration,
            py::arg("declaration").none(false))
        .def("addContract", &System::addContract,
            py::arg("contract").none(false))
        .def("addComponent", &System::addComponent,
            py::arg("component").none(false))
        .def("getDeclarationsSet", &System::getDeclarationsSet,
            py::return_value_policy::reference)
        .def("getContractsSet", &System::getContractsSet,
            py::return_value_policy::reference)
        .def("getComponentsSet", &System::getComponentsSet,
            py::return_value_policy::reference)
        .def("accept_visitor", &System::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &System::getString)
        .def("clone", &System::clone)
        .def("getName", &System::getName,
            py::return_value_policy::reference)
        .def("setName", &System::setName,
            py::arg("name").none(false));
    
    /**
    * CONTRACT BINDINGS
    */ 

    py::class_<Contract, std::unique_ptr<Contract, 
        py::nodelete>, Scope>(m, "Contract")
        .def(py::init<std::string >(),
            py::arg("name")="contract")
        .def("getName", &Contract::getName,
            py::return_value_policy::reference)
        .def("setName", &Contract::setName,
            py::arg("name").none(false))
        .def("addDeclaration", &Contract::addDeclaration,
            py::arg("declaration").none(false))
        .def("addAssumptions", &Contract::addAssumptions,
            py::arg("domain").none(false),
            py::arg("spec").none(false))
        .def("addGuarantees", &Contract::addGuarantees,
            py::arg("domain").none(false),
            py::arg("spec").none(false))
        .def("accept_visitor", &Contract::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Contract::getString)
        .def("clone", &Contract::clone)
        .def_static("saturate", &Contract::saturate,
            py::arg("c").none(false))
        .def_static("composition", &Contract::composition,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("correspondences").none(false),
            py::arg("name")="composition")
        .def_static("composeLogic", &Contract::composeLogic,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false))
        .def_static("conjunction", &Contract::conjunction,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("correspondences").none(false),
            py::arg("name")="conjunction")
        .def_static("conjoinLogic", &Contract::conjoinLogic,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false))
        .def_static("mergeDeclarations", 
            &Contract::mergeDeclarations,
            py::arg("c1").none(false),
            py::arg("c2").none(false),
            py::arg("r").none(false),
            py::arg("correspondences").none(false),
            py::arg("declaration_map").none(false))
        .def_static("saturateLogic", &Contract::saturateLogic,
            py::arg("c").none(false))
        .def_readwrite("declarations", &Contract::declarations)
        .def_readwrite("assumptions", &Contract::assumptions)
        .def_readwrite("guarantees", &Contract::guarantees);

    /**
    * COMPONENT BINDINGS
    */ 
    py::class_<Component, std::unique_ptr<Component,
        py::nodelete>, ChaseObject>(m, "Component")
        .def(py::init<ComponentDefinition *, std::string &>(),
            py::arg("definition").none(false),
            py::arg("name").none(false))
        .def("getDefinition", &Component::getDefinition)
        .def("setDefinition", &Component::setDefinition,
            py::arg("definition").none(false))
        .def("getName", &Component::getName)
        .def("setName", &Component::setName,
            py::arg("name").none(false))
        .def("getParameterValue", &Component::getParameterValue,
            py::arg("view").none(false),
            py::arg("param").none(false))
        .def("setParameter", &Component::setParameter,
            py::arg("view").none(false),
            py::arg("param").none(false),
            py::arg("value").none(false))
        .def("accept_visitor", &Component::accept_visitor,
            py::arg("v").none(false))
        .def("getString", &Component::getString)
        .def("clone", &Component::clone);

    /**
    * CHASE TIME BINDING
    */
    py::enum_<chase::chase_time_unit>(m, "chase_time_unit")
        .value("CHASE_SEC", chase::chase_time_unit::CHASE_SEC)
        .value("CHASE_MS", chase::chase_time_unit::CHASE_MS)
        .value("CHASE_US", chase::chase_time_unit::CHASE_US)
        .value("CHASE_NS", chase::chase_time_unit::CHASE_NS)
        .value("CHASE_PS", chase::chase_time_unit::CHASE_PS)
        .value("CHASE_FS", chase::chase_time_unit::CHASE_FS)
        .export_values();

    py::class_<chase_time, std::unique_ptr<chase_time, 
        py::nodelete>>(m, "chase_time")
        .def(py::init<unsigned int, chase_time_unit>(),
            py::arg("a")=1, py::arg("u")=CHASE_SEC)
        .def(py::init<unsigned int, std::string>(),
            py::arg("a"), py::arg("u"));

    /**
    * UTILITIES
    * BINDINGS
    *
    */

    /**
    * VISITOR BINDINGS
    */ 

       // Utilities library.
    auto u = k.def_submodule("utilities", 
        "Wrapper for the Chase utilities library.");

    /**
    * SIMPLIFY
    */

    u.def("simplify", &chase::simplify,
            py::arg("object").none(false),
            py::arg("options").none(false));


    py::class_<simplify_options, std::unique_ptr<simplify_options,
            py::nodelete>>(u, "simplify_options")
            .def(py::init<bool&, bool&>(),
                    py::arg("_nots")=true,
                    py::arg("_temporal_operators")=true)
            .def_readwrite("nots", &simplify_options::nots)
            .def_readwrite("temporal_operators",
                    &simplify_options::temporal_operators);

    /**
    * FACTORY BINDINGS
    */  
    u.def("True", &chase::True);
    u.def("False", &chase::False);
    u.def("Not", &chase::Not,
        py::arg("op").none(false));
    u.def("And", &chase::And,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Or", &chase::Or,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Implies", &chase::Implies,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Iff", &chase::Iff,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Nand", &chase::Nand,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Xor", &chase::Xor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Nor", &chase::Nor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Xnor", &chase::Xnor,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("LargeAnd", &chase::LargeAnd,
        py::arg("formulas").none(false));
    u.def("LargeOr", &chase::LargeOr,
        py::arg("formulas").none(false));
    u.def("Always", &chase::Always,
        py::arg("op").none(false));
    u.def("Eventually", &chase::Eventually,
        py::arg("op").none(false));
    u.def("Next", &chase::Next,
        py::arg("op").none(false));
    u.def("Until", &chase::Until,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Sum", &chase::Sum,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Sub", &chase::Sub,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Mult", &chase::Mult,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Div", &chase::Div,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Eq", &chase::Eq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("NEq", &chase::NEq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Eq", &chase::Eq,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("LE", &chase::LE,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("LT", &chase::LT,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("GE", &chase::GE,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("GT", &chase::GT,
        py::arg("op1").none(false),
        py::arg("op2").none(false));
    u.def("Prop", overload_cast_<Variable*>()
        (&chase::Prop), 
        py::arg("var").none(false));
    u.def("Prop", overload_cast_<Expression*>()
        (&chase::Prop), py::arg("exp").none(false));
    u.def("Id", &chase::Id,
        py::arg("declaration").none(false));
    u.def("intVal", &chase::IntVal,
        py::arg("n").none(false));
    u.def("RealVal", &chase::RealVal,
        py::arg("r").none(false));
    u.def("BoolVal", &chase::BoolVal,
        py::arg("b").none(false));    


    // Messagges and IO Utils.
    u.def("messageError", overload_cast_<const char *, chase::ChaseObject *>()
        (&chase::messageError),
        py::arg("msg").none(false),
        py::arg("object").none(false));
    u.def("messageError", overload_cast_<std::string, chase::ChaseObject *>()
        (&chase::messageError),
        py::arg("msg").none(false),
        py::arg("object").none(false));

    u.def("messageWarning", overload_cast_<const char *, chase::ChaseObject *>()
        (&chase::messageWarning),
        py::arg("msg").none(false),
        py::arg("object").none(false));
    u.def("messageWarning", overload_cast_<std::string, chase::ChaseObject *>()
        (&chase::messageWarning),
        py::arg("msg").none(false),
        py::arg("object").none(false));

    u.def("messageInfo", overload_cast_<const char *, chase::ChaseObject *>()
        (&chase::messageInfo),
        py::arg("msg").none(false),
        py::arg("object").none(false));
    u.def("messageInfo", overload_cast_<std::string, chase::ChaseObject *>()
        (&chase::messageError),
        py::arg("msg").none(false),
        py::arg("object").none(false));

    // Base Visitor bindig.

    py::class_<BaseVisitor, std::unique_ptr<BaseVisitor, py::nodelete>>
        (u, "BaseVisitor")
        .def(py::init<>())
        .def("visitBinaryBooleanOperation", &BaseVisitor::visitBinaryBooleanOperation, py::arg("v").none(false))
        .def("visitBinaryTemporalOperation", &BaseVisitor::visitBinaryTemporalOperation, py::arg("v").none(false))
        .def("visitBoolean", &BaseVisitor::visitBoolean, py::arg("v").none(false))
        .def("visitBooleanConstant", &BaseVisitor::visitBooleanConstant, py::arg("v").none(false))
        .def("visitBooleanValue", &BaseVisitor::visitBooleanValue, py::arg("v").none(false))
        .def("visitComponent", &BaseVisitor::visitComponent, py::arg("v").none(false))
        .def("visitComponentDefinition", &BaseVisitor::visitComponentDefinition, py::arg("v").none(false))
        .def("visitConstant", &BaseVisitor::visitConstant, py::arg("v").none(false))
        .def("visitConstraint", &BaseVisitor::visitConstraint, py::arg("v").none(false))
        .def("visitContract", &BaseVisitor::visitContract, py::arg("v").none(false))
        .def("visitCustomType", &BaseVisitor::visitCustomType, py::arg("v").none(false))
        .def("visitDesignProblem", &BaseVisitor::visitDesignProblem, py::arg("v").none(false))
        .def("visitDistribution", &BaseVisitor::visitDistribution, py::arg("v").none(false))
        .def("visitEdge", &BaseVisitor::visitEdge, py::arg("v").none(false))
        .def("visitEnumeration", &BaseVisitor::visitEnumeration, py::arg("v").none(false))
        .def("visitExpression", &BaseVisitor::visitExpression, py::arg("v").none(false))
        .def("visitFunction", &BaseVisitor::visitFunction, py::arg("v").none(false))
        .def("visitFunctionCall", &BaseVisitor::visitFunctionCall, py::arg("v").none(false))
        .def("visitGraph", &BaseVisitor::visitGraph, py::arg("v").none(false))
        .def("visitIdentifier", &BaseVisitor::visitIdentifier, py::arg("v").none(false))
        .def("visitInteger", &BaseVisitor::visitInteger, py::arg("v").none(false))
        .def("visitIntegerValue", &BaseVisitor::visitIntegerValue, py::arg("v").none(false))
        .def("visitInterval", &BaseVisitor::visitInterval, py::arg("v").none(false))
        .def("visitLargeBooleanFormula", &BaseVisitor::visitLargeBooleanFormula, py::arg("v").none(false))
        .def("visitLibrary", &BaseVisitor::visitLibrary, py::arg("v").none(false))
        .def("visitMatrix", &BaseVisitor::visitMatrix, py::arg("v").none(false))
        .def("visitModalFormula", &BaseVisitor::visitModalFormula, py::arg("v").none(false))
        .def("visitName", &BaseVisitor::visitName, py::arg("v").none(false))
        .def("visitParameter", &BaseVisitor::visitParameter, py::arg("v").none(false))
        .def("visitProbabilityFunction", &BaseVisitor::visitProbabilityFunction, py::arg("v").none(false))
        .def("visitProposition", &BaseVisitor::visitProposition, py::arg("v").none(false))
        .def("visitQuantifiedFormula", &BaseVisitor::visitQuantifiedFormula, py::arg("v").none(false))
        .def("visitRange", &BaseVisitor::visitRange, py::arg("v").none(false))
        .def("visitReal", &BaseVisitor::visitReal, py::arg("v").none(false))
        .def("visitRealValue", &BaseVisitor::visitRealValue, py::arg("v").none(false))
        .def("visitString", &BaseVisitor::visitString, py::arg("v").none(false))
        .def("visitStringValue", &BaseVisitor::visitStringValue, py::arg("v").none(false))
        .def("visitSystem", &BaseVisitor::visitSystem, py::arg("v").none(false))
        .def("visitUnaryBooleanOperation", &BaseVisitor::visitUnaryBooleanOperation, py::arg("v").none(false))
        .def("visitUnaryTemporalOperation", &BaseVisitor::visitUnaryTemporalOperation, py::arg("v").none(false))
        .def("visitVariable", &BaseVisitor::visitVariable, py::arg("v").none(false))
        .def("visitVertex", &BaseVisitor::visitVertex, py::arg("v").none(false));

    // Guide visitor binding.
    py::class_<GuideVisitor, std::unique_ptr<GuideVisitor, py::nodelete>, BaseVisitor>
        (u, "GuideVisitor")
        .def(py::init<int>(), py::arg("rv")=0)
        .def("visitBinaryBooleanOperation", &GuideVisitor::visitBinaryBooleanOperation, py::arg("v").none(false))
        .def("visitBinaryTemporalOperation", &GuideVisitor::visitBinaryTemporalOperation, py::arg("v").none(false))
        .def("visitBoolean", &GuideVisitor::visitBoolean, py::arg("v").none(false))
        .def("visitBooleanConstant", &GuideVisitor::visitBooleanConstant, py::arg("v").none(false))
        .def("visitBooleanValue", &GuideVisitor::visitBooleanValue, py::arg("v").none(false))
        .def("visitComponent", &GuideVisitor::visitComponent, py::arg("v").none(false))
        .def("visitComponentDefinition", &GuideVisitor::visitComponentDefinition, py::arg("v").none(false))
        .def("visitConstant", &GuideVisitor::visitConstant, py::arg("v").none(false))
        .def("visitConstraint", &GuideVisitor::visitConstraint, py::arg("v").none(false))
        .def("visitContract", &GuideVisitor::visitContract, py::arg("v").none(false))
        .def("visitCustomType", &GuideVisitor::visitCustomType, py::arg("v").none(false))
        .def("visitDesignProblem", &GuideVisitor::visitDesignProblem, py::arg("v").none(false))
        .def("visitDistribution", &GuideVisitor::visitDistribution, py::arg("v").none(false))
        .def("visitEdge", &GuideVisitor::visitEdge, py::arg("v").none(false))
        .def("visitEnumeration", &GuideVisitor::visitEnumeration, py::arg("v").none(false))
        .def("visitExpression", &GuideVisitor::visitExpression, py::arg("v").none(false))
        .def("visitFunction", &GuideVisitor::visitFunction, py::arg("v").none(false))
        .def("visitFunctionCall", &GuideVisitor::visitFunctionCall, py::arg("v").none(false))
        .def("visitGraph", &GuideVisitor::visitGraph, py::arg("v").none(false))
        .def("visitIdentifier", &GuideVisitor::visitIdentifier, py::arg("v").none(false))
        .def("visitInteger", &GuideVisitor::visitInteger, py::arg("v").none(false))
        .def("visitIntegerValue", &GuideVisitor::visitIntegerValue, py::arg("v").none(false))
        .def("visitInterval", &GuideVisitor::visitInterval, py::arg("v").none(false))
        .def("visitLargeBooleanFormula", &GuideVisitor::visitLargeBooleanFormula, py::arg("v").none(false))
        .def("visitLibrary", &GuideVisitor::visitLibrary, py::arg("v").none(false))
        .def("visitMatrix", &GuideVisitor::visitMatrix, py::arg("v").none(false))
        .def("visitModalFormula", &GuideVisitor::visitModalFormula, py::arg("v").none(false))
        .def("visitName", &GuideVisitor::visitName, py::arg("v").none(false))
        .def("visitParameter", &GuideVisitor::visitParameter, py::arg("v").none(false))
        .def("visitProbabilityFunction", &GuideVisitor::visitProbabilityFunction, py::arg("v").none(false))
        .def("visitProposition", &GuideVisitor::visitProposition, py::arg("v").none(false))
        .def("visitQuantifiedFormula", &GuideVisitor::visitQuantifiedFormula, py::arg("v").none(false))
        .def("visitRange", &GuideVisitor::visitRange, py::arg("v").none(false))
        .def("visitReal", &GuideVisitor::visitReal, py::arg("v").none(false))
        .def("visitRealValue", &GuideVisitor::visitRealValue, py::arg("v").none(false))
        .def("visitString", &GuideVisitor::visitString, py::arg("v").none(false))
        .def("visitStringValue", &GuideVisitor::visitStringValue, py::arg("v").none(false))
        .def("visitSystem", &GuideVisitor::visitSystem, py::arg("v").none(false))
        .def("visitUnaryBooleanOperation", &GuideVisitor::visitUnaryBooleanOperation, py::arg("v").none(false))
        .def("visitUnaryTemporalOperation", &GuideVisitor::visitUnaryTemporalOperation, py::arg("v").none(false))
        .def("visitVariable", &GuideVisitor::visitVariable, py::arg("v").none(false))
        .def("visitVertex", &GuideVisitor::visitVertex, py::arg("v").none(false));

    py::class_<ClonedDeclarationVisitor, 
        std::unique_ptr<ClonedDeclarationVisitor, py::nodelete>, 
        GuideVisitor>(u, "ClonedDeclarationVisitor")
        .def(py::init< std::map< Declaration *, Declaration * >& >(),
            py::arg("m").none(false))
        .def("visitIdentifier", &ClonedDeclarationVisitor::visitIdentifier,
            py::arg("o").none(false));

    u.def("getSubGraph", &chase::getSubGraph,
        py::arg("graph").none(false),
        py::arg("vertexes").none(false));

    u.def("findAllPathsBetweenNodes", &chase::findAllPathsBetweenNodes,
        py::arg("graph").none(false),
        py::arg("visited").none(false),
        py::arg("end").none(false),
        py::arg("result").none(false));

    py::class_<LogicSimplificationVisitor,
        std::unique_ptr<LogicSimplificationVisitor, py::nodelete>,
        GuideVisitor>(u, "LogicSimplificationVisitor")
        .def("visitBinaryBooleanOperation", 
            &LogicSimplificationVisitor::visitBinaryBooleanOperation,
            py::arg("formula").none(false))
        .def("visitUnaryBooleanOperation", 
            &LogicSimplificationVisitor::visitUnaryBooleanOperation,
            py::arg("formula").none(false))
        .def("visitLargeBooleanFormula", 
            &LogicSimplificationVisitor::visitLargeBooleanFormula,
            py::arg("formula").none(false))
        .def("visitUnaryTemporalOperation", 
            &LogicSimplificationVisitor::visitUnaryTemporalOperation,
            py::arg("formula").none(false))
        .def("visitBinaryTemporalOperation", 
            &LogicSimplificationVisitor::visitBinaryTemporalOperation,
            py::arg("formula").none(false));

    py::class_<GroupTemporalOperatorsVisitor,
        std::unique_ptr<GroupTemporalOperatorsVisitor, py::nodelete>,
        LogicSimplificationVisitor>(u, "GroupTemporalOperatorsVisitor")
        .def(py::init<>());

    py::class_<LogicNotNormalizationVisitor,
        std::unique_ptr<LogicNotNormalizationVisitor, py::nodelete>,
        LogicSimplificationVisitor>(u, "LogicNotNormalizationVisitor")
        .def(py::init<>());

    py::class_<VarsCausalityVisitor,
        std::unique_ptr<VarsCausalityVisitor, py::nodelete>,
        GuideVisitor>(u, "VarsCausalityVisitor")
        .def(py::init<Contract*>(),
            py::arg("contract")=nullptr)
        .def("getContract", &VarsCausalityVisitor::getContract)
        .def("setContract", &VarsCausalityVisitor::setContract,
            py::arg("Contract").none(false))
        .def("visitVariable", &VarsCausalityVisitor::visitVariable,
            py::arg("variable").none(false))
        .def("visitContract", &VarsCausalityVisitor::visitContract,
            py::arg("contract").none(false))
        .def("visitIdentifier", &VarsCausalityVisitor::visitIdentifier,
            py::arg("identifier").none(false));
    
}

