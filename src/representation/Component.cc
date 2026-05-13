/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include <string>
#include "representation/Component.hh"


using namespace chase;

Component::Component( ComponentDefinition * definition, std::string name ) :
    _definition(definition),
    _name(new Name(name))
{
    _node_type = component_node;
    _name->setParent(this);
}

Component::~Component() = default;

ComponentDefinition * Component::getDefinition() const {
    return _definition;
}

void Component::setDefinition(ComponentDefinition * definition) {
    _definition = definition;
}

Name *Component::getName() const {
    return _name.get();
}

void Component::setName(Name *name) {
    if(_name.get() == name) {
        if(_name != nullptr) _name->setParent(this);
        return;
    }
    _name.reset(name);
    if(_name != nullptr) _name->setParent(this);
}

void
Component::setParameter(std::string view, std::string param, Value *value)
{
    auto & params = _params[view];
    auto it = params.find(param);
    if(it == params.end())
        params.emplace(param, std::unique_ptr<Value>(value));
    else
        it->second.reset(value);
    if(value != nullptr) value->setParent(this);
}

Value *Component::getParameterValue(std::string view, std::string param)
{
    auto v = _params.find(view);
    if( v == _params.end() ) return nullptr;

    auto p = v->second.find(param);
    if( p == v->second.end() ) return nullptr;

    return p->second.get();
}

std::map<std::string, std::unique_ptr<Value>>&
    Component::getParametersInView(std::string view)
{
    return _params[view];
}

int Component::accept_visitor(chase::BaseVisitor &v) {
    return v.visitComponent(*this);
}

std::string Component::getString() {
    std::string ret = "Component: " + _name->getString() + " is " +
                      _definition->getName()->getString() + "\n";

    for (auto i = _params.begin(); i != _params.end(); ++i) {
        ret += "view : " + i->first + "\n";
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            std::string param = j->first;
            std::string val = j->second != nullptr ? j->second->getString() : "";

            ret += ("parameter " + param + " = " + val + "\n");
        }
    }
    return ret;
}

Component *Component::clone() {
    auto ret = new Component(_definition, _name->getString());

    for(auto i = _params.begin(); i != _params.end(); ++i)
    {
        std::string view = i->first;
        for( auto j = i->second.begin(); j != i->second.end(); ++j )
        {
            std::string param = j->first;
            Value * val = j->second != nullptr ? j->second->clone() : nullptr;
            ret->setParameter(view, param, val);
        }
    }

    return ret;
}
