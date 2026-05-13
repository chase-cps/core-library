/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/11/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/ProbabilityFunction.hh"
#include "representation/Real.hh"

using namespace chase;

ProbabilityFunction::ProbabilityFunction(Specification * specification) :
    Value(),
    _specification(specification)
{
    _node_type  = probabilityFunction_node;
    setType(new Real(0.0, 1.0));
    if(_specification != nullptr) _specification->setParent(this);
}

ProbabilityFunction::~ProbabilityFunction() = default;

int ProbabilityFunction::accept_visitor(BaseVisitor &v) {
    return v.visitProbabilityFunction(*this);
}

std::string ProbabilityFunction::getString() {
    std::string ret("P(");
    if(_specification != nullptr) ret += _specification->getString();
    ret += ")";
    return ret;
}

ProbabilityFunction *ProbabilityFunction::clone()
{
    auto ret = new ProbabilityFunction(
            _specification != nullptr ? _specification->clone() : nullptr);
    return ret;
}

Specification *ProbabilityFunction::getSpecification() const
{
    return _specification.get();
}

void ProbabilityFunction::setSpecification(Specification *specification)
{
    if(_specification.get() == specification) {
        if(_specification != nullptr) _specification->setParent(this);
        return;
    }
    _specification.reset(specification);
    if(_specification != nullptr) _specification->setParent(this);
}

std::unique_ptr<Specification> ProbabilityFunction::releaseSpecification()
{
    auto ret = std::move(_specification);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
