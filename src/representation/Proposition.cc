/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "representation/Proposition.hh"
#include "chase-core.hh"

using namespace chase;

Proposition::Proposition() :
    _type(new Boolean()),
    _value(nullptr),
    _name(nullptr)
{
    _node_type = proposition_node;
    _type->setParent(this);
}

Proposition::~Proposition()
    = default;

Proposition::Proposition(Value *v) :
    _type(new Boolean()),
    _value(v),
    _name(nullptr)
{
    _node_type = proposition_node;

    switch( _value->IsA()) {
        case (identifier_node) :
            Identifier *id;
            id = dynamic_cast< Identifier * >(v);
            if (id != nullptr) {
                _name.reset(new Name(id->getDeclaration()->getName()->getString()));
                _name->setParent(this);
            }
            else messageError("Something went wrong in dynamic cast.");
            if (id->getType()->IsA() != boolean_node)
                messageError(
                        "Not a boolean typed variable or constant when creating a proposition",
                        id);
            break;
        case (expression_node) :
            Expression *exp;
            exp = dynamic_cast< Expression * >(v);
            if (exp != nullptr)
                if (exp->getType()->IsA() != boolean_node)
                    messageError(
                            "Not a boolean typed expression when creating a proposition",
                            exp);
            break;
        default:
            messageError("Not boolean object when creating a proposition", v);
            break;
    }
    if(_value != nullptr && _value->getParent() == nullptr) _value->setParent(this);

}

Type *Proposition::getType() {
    return _type.get();
}

Value *Proposition::getValue() {
    return _value.get();
}

void Proposition::setValue(Value *v) {
    if(_value.get() == v) {
        if(_value != nullptr && _value->getParent() == nullptr )
            _value->setParent(this);
        return;
    }
    _value.reset(v);
    if(_value != nullptr && _value->getParent() == nullptr ) _value->setParent(this);
}

Name *Proposition::getName() {
    return _name.get();
}

void Proposition::setName(Name *n) {
    if(_name.get() == n) {
        if(_name != nullptr && _name->getParent() == nullptr )
            _name->setParent(this);
        return;
    }
    _name.reset(n);
    if(_name != nullptr && _name->getParent() == nullptr ) _name->setParent(this);
}

int Proposition::accept_visitor(chase::BaseVisitor &v) {
    return v.visitProposition(*this);
}

std::string Proposition::getString() {
    std::string ret;
    if( _value->IsA() == identifier_node )
        ret = _name->getString();
    else if( _value->IsA() == expression_node )
        ret = static_cast< Expression * >(_value.get())->getString();
    return ret;
}

Proposition * Proposition::clone()
{
    auto ret = new Proposition();
    if( _name != nullptr ) ret->setName(_name->clone());
    if( _value != nullptr ) ret->setValue(_value->clone());
    return ret;
}
