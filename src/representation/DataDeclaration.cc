/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#include "representation/DataDeclaration.hh"

using namespace chase;

DataDeclaration::DataDeclaration( Type * t, Name * n ) :
    Declaration(n),
    _type(t)
{
    _node_type = dataDeclaration_node;
    if(_name != nullptr) _name->setParent(this);
    if(_type != nullptr) _type->setParent(this);
}

Type * DataDeclaration::getType()
{
    return _type.get();
}

void DataDeclaration::setType( Type * t )
{
    if(_type.get() == t) {
        if(_type != nullptr) _type->setParent(this);
        return;
    }
    _type.reset(t);
    if(_type != nullptr) _type->setParent(this);
}
