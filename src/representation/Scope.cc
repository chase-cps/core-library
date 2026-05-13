/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/14/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/Scope.hh"

using namespace chase;

void Scope::addDeclaration(Declaration *declaration) {
    if(declaration == nullptr) return;
    for(auto & owned : declarations)
        if(owned.get() == declaration) return;
    declarations.emplace_back(declaration);
    declaration->setParent(this);
}

Scope::Scope(Name *n) :
    Declaration(n)
{
}

Scope::Scope(std::string name) :
    Declaration(name)
{
}

Scope::Scope() {}

