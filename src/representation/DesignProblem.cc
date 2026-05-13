/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/DesignProblem.hh"
#include "representation/Library.hh"
#include "representation/Contract.hh"
#include "representation/System.hh"

using namespace chase;

DesignProblem::DesignProblem() :
    ChaseObject()
{
    _node_type = design_problem_node;
}

DesignProblem::~DesignProblem() = default;

int DesignProblem::accept_visitor(BaseVisitor &v) {
    return v.visitDesignProblem(*this);
}

std::string DesignProblem::getString() {
    std::string ret("");
    return ret;
}

DesignProblem *DesignProblem::clone() {
    auto ret = new DesignProblem();
    for(auto & it : libraries)
        ret->libraries.emplace(it->clone());
    for(auto & it : requirements)
        ret->requirements.emplace(it->clone());
    if(_system != nullptr) ret->setSystem(_system->clone());
    return ret;
}

System *DesignProblem::getSystem() const {
    return _system.get();
}

void DesignProblem::setSystem(System *system) {
    _system.reset(system);
    if(system != nullptr) system->setParent(this);
}
