/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        6/20/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "representation/CustomType.hh"

using namespace chase;

CustomType::CustomType(std::string name, Type *type) :
    CustomType(new Name(name), type)
{
}

CustomType::CustomType(Name *name, Type *type) :
    Type(),
    _name(name),
    _type(type)
{
    _node_type = customType_node;
    if(_name == nullptr) _name.reset(new Name(""));
    if(_name != nullptr) _name->setParent(this);
    if(_type != nullptr) _type->setParent(this);
}

Type *CustomType::getType() const {
    return _type.get();
}

void CustomType::setType(Type *type) {
    if(_type.get() == type) {
        if(_type != nullptr) _type->setParent(this);
        return;
    }
    _type.reset(type);
    if(_type != nullptr) _type->setParent(this);
}

Name *CustomType::getName() const {
    return _name.get();
}

void CustomType::setName(Name *name) {
    if(_name.get() == name) {
        if(_name != nullptr) _name->setParent(this);
        return;
    }
    _name.reset(name);
    if(_name != nullptr) _name->setParent(this);
}

std::string CustomType::getString() {
    std::string ret("CUSTOM TYPE: ");
    if(_name != nullptr) ret += _name->getString();
    return ret;
}

int CustomType::accept_visitor(BaseVisitor &v) {
    return v.visitCustomType(*this);
}

Type *CustomType::clone() {
    return new CustomType(
            _name != nullptr ? _name->clone() : nullptr,
            _type != nullptr ? _type->clone() : nullptr);
}

CustomType::~CustomType() = default;
