/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Expression.hh"

using namespace chase;

Expression::Expression() :
    Value(),
    _op(op_none),
    _op1(nullptr),
    _op2(nullptr)
{
    _node_type = expression_node;
    if(_op1 != nullptr) _op1->setParent(this);
    if(_op2 != nullptr) _op2->setParent(this);
}

Expression::Expression(Operator op, Value *op1, Value *op2 ) :
    Value(),
    _op(op),
    _op1(op1),
    _op2(op2)
{
    _node_type = expression_node;
    if(_op1 != nullptr) _op1->setParent(this);
    if(_op2 != nullptr) _op2->setParent(this);
}

Expression::~Expression() = default;


Operator Expression::getOperator()
{
    return _op;
}

Value * Expression::getOp1()
{
    return _op1.get();
}

Value * Expression::getOp2()
{
    return _op2.get();
}

void Expression::setOperator(Operator op )
{
    _op = op;
}

void Expression::setOp1(Value * op )
{
    if(_op1.get() == op) {
        if(_op1 != nullptr) _op1->setParent(this);
        return;
    }
    _op1.reset(op);
    if(_op1 != nullptr) _op1->setParent(this);
}

void Expression::setOp2(Value * op )
{
    if(_op2.get() == op) {
        if(_op2 != nullptr) _op2->setParent(this);
        return;
    }
    _op2.reset(op);
    if(_op2 != nullptr) _op2->setParent(this);
}

std::string Expression::getString()
{
    std::string ret = _op1->getString();
    ret += to_string(_op);
    ret += _op2->getString();
    return ret;
}

int Expression::accept_visitor(BaseVisitor &v )
{
    return v.visitExpression(*this);
}

Type * Expression::getType()
{
    if( _op >= op_eq && _op <= op_ge )
        return new Boolean();
    else
        return nullptr;
    /// \todo Fix this to find the actual type.
}

Expression *Expression::clone() {
    return new Expression(
            _op,
            _op1 != nullptr ? _op1->clone() : nullptr,
            _op2 != nullptr ? _op2->clone() : nullptr);
}

std::unique_ptr<Value> Expression::releaseOp1()
{
    auto ret = std::move(_op1);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}

std::unique_ptr<Value> Expression::releaseOp2()
{
    auto ret = std::move(_op2);
    if(ret != nullptr) ret->setParent(nullptr);
    return ret;
}
