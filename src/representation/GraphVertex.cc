/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "representation/Graph.hh"
using namespace chase;

Vertex::Vertex(Name *name) :
    _name(name)
{
    _node_type = graphVertex_node;
    if( _name == nullptr )
    {
        _name.reset(new Name("GenericVertex"));
    }
    _name->setParent(this);
}

Vertex::~Vertex() = default;

Name *Vertex::getName() const {
    return _name.get();
}

void Vertex::setName(std::string name) {
    _name.reset(new Name(std::move(name)));
    _name->setParent(this);
}

int Vertex::accept_visitor(chase::BaseVisitor &v) {
    return v.visitVertex(*this);
}

std::string Vertex::getString() {
    return _name->getString();
}


std::string Vertex::getGraphViz() {
    return std::string();
}

Vertex *Vertex::clone() {
    return new Vertex(_name->clone());
}
