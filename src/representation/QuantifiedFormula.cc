/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/22/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/QuantifiedFormula.hh"

using namespace chase;

QuantifiedFormula::QuantifiedFormula(
        logic_quantifier quantifier,
        Variable *variable,
        LogicFormula *formula) :
    _quantifier(quantifier),
    _variable(variable),
    _formula(formula)
{
    _node_type = quantifiedFormula_node;
    if(_variable == nullptr) _variable.reset(new Variable());
    if(_variable != nullptr) _variable->setParent(this);
    if(_formula != nullptr) _formula->setParent(this);
}

QuantifiedFormula::~QuantifiedFormula() = default;

int QuantifiedFormula::accept_visitor(BaseVisitor &v) {
    return v.visitQuantifiedFormula(*this);
}

std::string QuantifiedFormula::getString() {
    std::string ret("");
    return ret;
}

QuantifiedFormula *QuantifiedFormula::clone() {
    auto ret = new QuantifiedFormula(
            _quantifier,
            _variable != nullptr ? _variable->clone() : nullptr,
            _formula != nullptr ? _formula->clone() : nullptr);
    return ret;
}

logic_quantifier QuantifiedFormula::getQuantifier() const {
    return _quantifier;
}

void QuantifiedFormula::setQuantifier(logic_quantifier quantifier) {
    _quantifier = quantifier;
}

Variable *QuantifiedFormula::getVariable() const {
    return _variable.get();
}

void QuantifiedFormula::setVariable(Variable *variable) {
    if(_variable.get() == variable) {
        if(_variable != nullptr) _variable->setParent(this);
        return;
    }
    _variable.reset(variable);
    if(_variable != nullptr) _variable->setParent(this);
}

LogicFormula *QuantifiedFormula::getFormula() const {
    return _formula.get();
}

void QuantifiedFormula::setFormula(LogicFormula *formula) {
    if(_formula.get() == formula) {
        if(_formula != nullptr) _formula->setParent(this);
        return;
    }
    _formula.reset(formula);
    if(_formula != nullptr) _formula->setParent(this);
}

std::unique_ptr<Variable> QuantifiedFormula::releaseVariable()
{
    auto ret = std::move(_variable);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<LogicFormula> QuantifiedFormula::releaseFormula()
{
    auto ret = std::move(_formula);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}


