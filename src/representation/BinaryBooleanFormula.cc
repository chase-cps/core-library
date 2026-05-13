/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/BinaryBooleanFormula.hh"

using namespace chase;

chase::BinaryBooleanFormula::BinaryBooleanFormula(
        BooleanOperator op, LogicFormula *op1, LogicFormula *op2) :
        _op(op),
        _op1(op1),
        _op2(op2)
{
    _node_type = binaryBooleanOperation_node;
    if(_op1 != nullptr) _op1->setParent(this);
    if(_op2 != nullptr) _op2->setParent(this);
}

BooleanOperator BinaryBooleanFormula::getOp() const {
    return _op;
}

void BinaryBooleanFormula::setOp(BooleanOperator op) {
    _op = op;
}

LogicFormula *BinaryBooleanFormula::getOp1() const {
    return _op1.get();
}

void BinaryBooleanFormula::setOp1(LogicFormula *op1) {
    if(_op1.get() == op1) {
        if(_op1 != nullptr) _op1->setParent(this);
        return;
    }
    _op1.reset(op1);
    if(_op1 != nullptr) _op1->setParent(this);
}

LogicFormula *BinaryBooleanFormula::getOp2() const {
    return _op2.get();
}

void BinaryBooleanFormula::setOp2(LogicFormula *op2) {
    if(_op2.get() == op2) {
        if(_op2 != nullptr) _op2->setParent(this);
        return;
    }
    _op2.reset(op2);
    if(_op2 != nullptr) _op2->setParent(this);
}

int BinaryBooleanFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBinaryBooleanOperation(*this);
}

std::string BinaryBooleanFormula::getString() {
    std::string ret = "(";
    ret += _op1->getString();
    ret += to_string(_op);
    ret += _op2->getString();
    ret += ")";
    return ret;
}

BinaryBooleanFormula *BinaryBooleanFormula::clone()
{
    return new BinaryBooleanFormula(
            _op,
            _op1 != nullptr ? _op1->clone() : nullptr,
            _op2 != nullptr ? _op2->clone() : nullptr);
}

BinaryBooleanFormula::~BinaryBooleanFormula() = default;

std::unique_ptr<LogicFormula> BinaryBooleanFormula::releaseOp1()
{
    auto ret = std::move(_op1);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<LogicFormula> BinaryBooleanFormula::releaseOp2()
{
    auto ret = std::move(_op2);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
