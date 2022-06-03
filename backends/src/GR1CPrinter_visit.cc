/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "GR1CPrinter.hh"

using namespace chase;

int GR1CPrinter::visitIntegerValue(IntegerValue &o) {
    _curr += std::to_string(o.getValue());
    return 1;
}

int GR1CPrinter::visitExpression(Expression &o) {
    /// \todo Fix to unroll.

    Operator op = o.getOperator();
    auto op1 = o.getOp1();
    auto op2 = o.getOp2();
    if (op == op_eq)
    {
        _curr += "(";
        _continueVisit(op1);
        _curr += to_string(op);

        if( op2->IsA() == integerValue_node) {
            _continueVisit(o.getOp2());
            _curr += ")";
        }
        else
        {
            /// \todo Unroll the expression.
            messageWarning(
                    std::string("Impossible to generate GR1C description.\n")
                + "Unsupported expression: " + op2->getString());
        }
    }
    else
    {
        /// \todo Unroll the expression.
        messageWarning(
                std::string("Impossible to generate GR1C description.\n")
                + "Unsupported expression: " + o.getString());
    }

    return 1;
}

int GR1CPrinter::visitIdentifier(Identifier &o) {
    if( _inNext && o.isPrimed())
        messageError("Primed identifier in the scope of a next operator: "
                     + o.getString());
    if(_inNext)
    {
        _curr += o.getString();
        _curr +="'";
    } else
        _curr += o.getString();
    return 1;
}

int GR1CPrinter::visitProposition(Proposition &o)
{
    if(o.getValue()->IsA() == identifier_node)
    {
        reinterpret_cast<Identifier*>(o.getValue())->accept_visitor(*this);
    }
    else if(o.getValue()->IsA() == expression_node)
    {
        reinterpret_cast<Expression*>(o.getValue())->accept_visitor(*this);
    }
    return 1;
}

int GR1CPrinter::visitBooleanConstant(BooleanConstant &o) {
    return GuideVisitor::visitBooleanConstant(o);
}

int GR1CPrinter::visitBinaryBooleanOperation(BinaryBooleanFormula &o) {
    _curr += "(";
    _continueVisit(o.getOp1());

    switch(o.getOp())
    {
        case op_not:
            messageError("Not a binary operator.");
            break;
        case op_and:
            _curr += " & ";
            break;
        case op_or:
            _curr += " | ";
            break;
        case op_implies:
            _curr += " -> ";
            break;
        case op_iff:
            _curr += " <-> ";
            break;
        case op_xor:
        case op_nand:
        case op_nor:
        case op_xnor:
            messageError("Operator not supported.");
            break;
    }
    _continueVisit(o.getOp2());
    _curr += ")";
    return 1;
}

int GR1CPrinter::visitUnaryBooleanOperation(UnaryBooleanFormula &o) {
    switch(o.getOp())
    {
        case op_not:
            _curr += "(!";
            _curr += "(";
            _continueVisit(o.getOp1());
            _curr += "))";
            break;
        case op_and:
            messageError("Not an Unary operator.");
            break;
        case op_or:
            messageError("Not an Unary operator.");
            break;
        case op_implies:
            messageError("Not an Unary operator.");
            break;
        case op_iff:
            messageError("Not an Unary operator.");
            break;
        case op_xor:
            messageError("Not an Unary operator.");
            break;
        case op_nand:
            messageError("Not an Unary operator.");
            break;
        case op_nor:
            messageError("Not an Unary operator.");
            break;
        case op_xnor:
            messageError("Not an Unary operator.");
            break;
    }
    return 1;
}

int GR1CPrinter::visitUnaryTemporalOperation(UnaryTemporalFormula &o) {
    switch(o.getOp()) {
        case op_next:
            if (_inNext == true)
                messageError("Illegal form: double annidated Next.");
            _inNext = true;
            _continueVisit(o.getFormula());
            _inNext = false;
            break;
        case op_globally:
            _curr += "[]";
            _continueVisit(o.getFormula());
            break;
        case op_future:
            _curr += "<>";
            _continueVisit(o.getFormula());
            break;
        case op_until:
            messageError("Until is not unary.");
            break;
        case op_release:
            messageError("Release operator is not unary.");
            break;
    }
    return 1;
}

int GR1CPrinter::visitBinaryTemporalOperation(BinaryTemporalFormula &o)
{
    switch(o.getOp()) {
        case op_next:
            messageError("Next operator is not binary.");
            break;
        case op_globally:
            messageError("Always operator is not binary.");
            break;
        case op_future:
            messageError("Eventually operator is not binary.");
            break;
        case op_until:
            messageError("Until is not currently supported.");
            break;
        case op_release:
            messageError("Release operator is not currently supported.");
            break;
    }
    return 1;
}

int GR1CPrinter::_continueVisit(ChaseObject *o) {
    switch(o->IsA())
    {
        case integerValue_node:{
            auto v = reinterpret_cast< IntegerValue * >(o);
            v->accept_visitor(*this);
            break;
        }
        case expression_node: {
                auto v = reinterpret_cast< Expression *>(o);
                v->accept_visitor(*this);
                break;
        }
        case identifier_node: {
            auto v = reinterpret_cast< Identifier * > (o);
            v->accept_visitor(*this);
            break;
        }
        case proposition_node: {
            auto v = reinterpret_cast< Proposition * > (o);
            v->accept_visitor(*this);
            break;
        }
        case booleanConstant_node: {
            auto v = reinterpret_cast< BooleanConstant * > (o);
            v->accept_visitor(*this);
            break;
        }
        case binaryBooleanOperation_node: {
            auto v = reinterpret_cast< BinaryBooleanFormula * > (o);
            v->accept_visitor(*this);
            break;
        }
        case unaryBooleanOperation_node: {
            auto v = reinterpret_cast< UnaryBooleanFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        case unaryTemporalOperation_node: {
            auto v = reinterpret_cast< UnaryTemporalFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        case binaryTemporalOperation_node: {
            auto v = reinterpret_cast< BinaryTemporalFormula *>(o);
            v->accept_visitor(*this);
            break;
        }
        default:
            messageError("Unsupported formula.");
            break;
    }
    return 1;
}






