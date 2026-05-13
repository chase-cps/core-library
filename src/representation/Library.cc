/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/3/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/Library.hh"

using namespace chase;

Library::Library() :
    Scope()
{
    _node_type = library_node;
}

Library::Library(const std::string &name) :
    Scope(name)
{
    _node_type = library_node;
}

Library::Library(Name *n) : Scope(n) {_node_type = library_node;}

Library::~Library() = default;

int Library::accept_visitor(BaseVisitor &v) {
    return v.visitLibrary(*this);
}

/// \todo Complete the print method, once the library is defined in the grammar.
std::string Library::getString() {
    std::string ret("Library");
    return ret;
}

Library *Library::clone() {
    auto ret = new Library();
    for(auto & it : declarations)
        ret->addDeclaration(it->clone());
    return ret;
}


