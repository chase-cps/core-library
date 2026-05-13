/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/30/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/ModalFormula.hh"
#include "utilities/IOUtils.hh"

using namespace chase;

ModalFormula::ModalFormula(ModalOperator op, LogicFormula * formula) :
    _operator(op),
    _formula(formula)
{
    _node_type = modalFormula_node;
    if( formula != nullptr && formula->getParent() != nullptr )
        messageWarning("Formula with parent already set:", formula);
    if(_formula != nullptr) _formula->setParent(this);
}

ModalFormula::~ModalFormula() = default;

ModalOperator ModalFormula::getOperator()
{
    return _operator;
}

void ModalFormula::setOperator(ModalOperator op) {
    _operator = op;
}

LogicFormula * ModalFormula::getFormula()
{
    return _formula.get();
}

void ModalFormula::setFormula(LogicFormula * formula)
{
    if(_formula.get() == formula) {
        if(_formula != nullptr) _formula->setParent(this);
        return;
    }
    _formula.reset(formula);
    if(_formula != nullptr) _formula->setParent(this);
}

int ModalFormula::accept_visitor(chase::BaseVisitor &v) {
    return v.visitModalFormula(*this);
}

std::string ModalFormula::getString() {
    std::string ret(to_string(_operator));
    ret += "(";
    ret += _formula->getString();
    ret += ")";
    return ret;
}

ModalFormula *ModalFormula::clone()
{
    return new ModalFormula(
            _operator, _formula != nullptr ? _formula->clone() : nullptr);
}

std::unique_ptr<LogicFormula> ModalFormula::releaseFormula()
{
    auto ret = std::move(_formula);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
