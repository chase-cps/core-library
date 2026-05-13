/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/Constraint.hh"

using namespace chase;

Constraint::Constraint() :
    Specification()
{
    _node_type = constraint_node;
}

Constraint::~Constraint() = default;

int Constraint::accept_visitor(BaseVisitor &v) {
    return v.visitConstraint(*this);
}

std::string Constraint::getString()
{
    std::string ret(_expression != nullptr ? _expression->getString() : "");
    return ret;
}

Constraint *Constraint::clone() {
    auto ret = new Constraint();
    ret->setExpression(_expression != nullptr ? _expression->clone() : nullptr);
    return ret;
}

Expression *Constraint::getExpression() const {
    return _expression.get();
}

void Constraint::setExpression(Expression *expression) {
    if(_expression.get() == expression) {
        if(_expression != nullptr) _expression->setParent(this);
        return;
    }
    _expression.reset(expression);
    if(_expression != nullptr) _expression->setParent(this);
}

std::unique_ptr<Expression> Constraint::releaseExpression()
{
    auto ret = std::move(_expression);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
