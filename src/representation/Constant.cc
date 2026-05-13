/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Constant.hh"
#include "representation/Value.hh"

using namespace chase;

Constant::Constant( Type * type, Name * name, Value * value ) :
    DataDeclaration( type, name ),
    _value(value)
{
    _node_type = constant_node;

    if(_name != nullptr) _name->setParent(this);
    if(_type != nullptr) _type->setParent(this);
    if(_value != nullptr) _value->setParent(this);
}

Constant::~Constant() = default;

Constant::Constant( const Constant &o ) :
    DataDeclaration(
            o._type != nullptr ? o._type->clone() : nullptr,
            o._name != nullptr ? o._name->clone() : nullptr),
    _value(o._value != nullptr ? o._value->clone() : nullptr)
{
    _node_type = constant_node;
    if(_name != nullptr) _name->setParent(this);
    if(_type != nullptr) _type->setParent(this);
    if(_value != nullptr) _value->setParent(this);
}

Constant & Constant::operator=(const Constant & )
{
    return *this;
}

std::string Constant::getString()
{
    std::string ret("constant: ");
    if( _name != nullptr && _type != nullptr )
    {
        ret += _name->getString();
        ret += " (";
        ret += _type->getString();
        ret += ") = ";
        ret += _value->getString();

    }
    else{
        ret = std::string("NULL CONSTANT");
    }
    return ret;
}

int Constant::accept_visitor( BaseVisitor &v )
{
    return v.visitConstant(*this);
}

void Constant::setValue(Value *value) {
    if(_value.get() == value) {
        if(_value != nullptr) _value->setParent(this);
        return;
    }
    _value.reset(value);
    if(_value != nullptr) _value->setParent(this);
}

Value *Constant::getValue() {
    return _value.get();
}

Constant * Constant::clone() {
    return new Constant(
            _type != nullptr ? _type->clone() : nullptr,
            _name != nullptr ? _name->clone() : nullptr,
            _value != nullptr ? _value->clone() : nullptr);
}
