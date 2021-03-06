/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/UnaryBooleanFormula.hh"
#include "representation/Operators.hh"

using namespace chase;

chase::UnaryBooleanFormula::UnaryBooleanFormula(
        BooleanOperator op, LogicFormula *op1) :
        _op(op),
        _op1(op1)
{
    _node_type = unaryBooleanOperation_node;
    _op1->setParent(this);
}

BooleanOperator UnaryBooleanFormula::getOp() const {
    return _op;
}

void UnaryBooleanFormula::setOp(BooleanOperator op) {
    _op = op;
}

LogicFormula *UnaryBooleanFormula::getOp1() const {
    return _op1;
}

void UnaryBooleanFormula::setOp1(LogicFormula *op1) {
    _op1 = op1;
    _op1->setParent(this);
}

int UnaryBooleanFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitUnaryBooleanOperation(*this);
}

std::string UnaryBooleanFormula::getString() {
    std::string ret;
    if(_op == op_not) ret += "NOT";
    ret += "(";
    ret += _op1->getString();
    ret += ")";
    return ret;
}

UnaryBooleanFormula * UnaryBooleanFormula::clone()
{
    return new UnaryBooleanFormula(_op, _op1->clone());
}

UnaryBooleanFormula::~UnaryBooleanFormula() = default;
