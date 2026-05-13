/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/Value.hh"

using namespace chase;

Value::Value() :
    _type(nullptr)
{
    _node_type = value_node;
}

Value::~Value()
{
}

Type *Value::getType() {
    return _type.get();
}

void Value::setType(Type *type) {
    if(_type.get() == type) {
        if(_type != nullptr) _type->setParent(this);
        return;
    }
    _type.reset(type);
    if(_type != nullptr) _type->setParent( this );
}

