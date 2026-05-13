/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/UnaryTemporalFormula.hh"
#include "representation/Operators.hh"

#include "utilities/IOUtils.hh"

using namespace chase;

UnaryTemporalFormula::UnaryTemporalFormula(
        TemporalOperator op,
        LogicFormula * formula,
        Interval * interval
        ) :
        _op(op),
        _formula(formula),
        _interval(interval)
{
    _node_type = unaryTemporalOperation_node;
    if(_formula != nullptr && _formula->getParent() != nullptr )
        messageWarning("Formula with already set parent", _formula.get());
    else if(_formula != nullptr) _formula->setParent(this);
    if(_interval != nullptr) {
        if(_interval->getParent() != nullptr){
            messageWarning("Interval with already set parent", _interval.get());
        } else _interval->setParent(this);
    }
}

TemporalOperator UnaryTemporalFormula::getOp() const {
    return _op;
}

void UnaryTemporalFormula::setOp(TemporalOperator op) {
    _op = op;
}

LogicFormula *UnaryTemporalFormula::getFormula() const {
    return _formula.get();
}

void UnaryTemporalFormula::setFormula(LogicFormula *formula) {
    if(_formula.get() == formula) {
        if(_formula != nullptr) _formula->setParent(this);
        return;
    }
    _formula.reset(formula);
    if(_formula != nullptr) _formula->setParent(this);
}

int UnaryTemporalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitUnaryTemporalOperation(*this);
}

std::string UnaryTemporalFormula::getString() {
    std::string ret(std::to_string(_op));
    if(_interval != nullptr)
        ret += _interval->getString();
    ret += "(";
    ret += _formula->getString();
    ret += ")";

    return ret;
}

UnaryTemporalFormula *UnaryTemporalFormula::clone() {
    return new UnaryTemporalFormula(
            _op,
            _formula != nullptr ? _formula->clone() : nullptr,
            _interval != nullptr ? _interval->clone() : nullptr);
}

Interval *UnaryTemporalFormula::getInterval() const {
    return _interval.get();
}

void UnaryTemporalFormula::setInterval(Interval *interval) {
    if(_interval.get() == interval) {
        if(_interval != nullptr) _interval->setParent(this);
        return;
    }
    _interval.reset(interval);
    if(_interval != nullptr) _interval->setParent(this);
}

UnaryTemporalFormula::~UnaryTemporalFormula() = default;

std::unique_ptr<LogicFormula> UnaryTemporalFormula::releaseFormula()
{
    auto ret = std::move(_formula);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<Interval> UnaryTemporalFormula::releaseInterval()
{
    auto ret = std::move(_interval);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
