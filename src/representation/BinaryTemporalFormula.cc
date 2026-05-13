/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/BinaryTemporalFormula.hh"
#include "representation/Operators.hh"

#include "utilities/IOUtils.hh"

using namespace chase;

BinaryTemporalFormula::BinaryTemporalFormula(
        TemporalOperator op,
        LogicFormula * op1,
        LogicFormula * op2,
        Interval * interval
) :
    _op(op),
    _formula1(op1),
    _formula2(op2),
    _interval(interval)
{
    _node_type = binaryTemporalOperation_node;

    if(_formula1 != nullptr && _formula1->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula1.get());
    else if(_formula1 != nullptr) _formula1->setParent(this);

    if(_formula2 != nullptr && _formula2->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula2.get());
    else if(_formula2 != nullptr) _formula2->setParent(this);

    if(_interval != nullptr) {
        if(_interval->getParent() != nullptr){
            messageWarning("Interval with already set parent", _interval.get());
        } else _interval->setParent(this);
    }
}

TemporalOperator BinaryTemporalFormula::getOp() const {
    return _op;
}

void BinaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

LogicFormula *BinaryTemporalFormula::getFormula1() const {
    return _formula1.get();
}

void BinaryTemporalFormula::setFormula1(LogicFormula *formula1) {
    if(_formula1.get() == formula1) {
        if(_formula1 != nullptr) _formula1->setParent(this);
        return;
    }
    _formula1.reset(formula1);
    if(_formula1 != nullptr) _formula1->setParent(this);
}

LogicFormula *BinaryTemporalFormula::getFormula2() const {
    return _formula2.get();
}

void BinaryTemporalFormula::setFormula2(LogicFormula *formula2) {
    if(_formula2.get() == formula2) {
        if(_formula2 != nullptr) _formula2->setParent(this);
        return;
    }
    _formula2.reset(formula2);
    if(_formula2 != nullptr) _formula2->setParent(this);
}

int BinaryTemporalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitBinaryTemporalOperation(*this);
}

std::string BinaryTemporalFormula::getString() {
    std::string ret(_formula1->getString());
    ret += to_string(_op);
    if(_interval != nullptr)
        ret += _interval->getString();
    ret += _formula2->getString();
    return ret;
}

BinaryTemporalFormula *BinaryTemporalFormula::clone()
{
    return new BinaryTemporalFormula(
            _op,
            _formula1 != nullptr ? _formula1->clone() : nullptr,
            _formula2 != nullptr ? _formula2->clone() : nullptr,
            _interval != nullptr ? _interval->clone() : nullptr);
}

Interval *BinaryTemporalFormula::getInterval() const {
    return _interval.get();
}

void BinaryTemporalFormula::setInterval(Interval *interval) {
    if(_interval.get() == interval) {
        if(_interval != nullptr) _interval->setParent(this);
        return;
    }
    _interval.reset(interval);
    if(_interval != nullptr) _interval->setParent(this);
}

BinaryTemporalFormula::~BinaryTemporalFormula() = default;

std::unique_ptr<LogicFormula> BinaryTemporalFormula::releaseFormula1()
{
    auto ret = std::move(_formula1);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<LogicFormula> BinaryTemporalFormula::releaseFormula2()
{
    auto ret = std::move(_formula2);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<Interval> BinaryTemporalFormula::releaseInterval()
{
    auto ret = std::move(_interval);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
