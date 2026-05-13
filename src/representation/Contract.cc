/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include <utility>

#include "representation/Contract.hh"
#include "utilities/ClonedDeclarationVisitor.hh"

using namespace chase;

Contract::Contract( std::string name ) :
    Scope(std::move(name))
{
    _node_type = contract_node;
}

Contract::~Contract() = default;

int Contract::accept_visitor(chase::BaseVisitor &v) {
    return v.visitContract(*this);
}

std::string Contract::getString() {
    std::string ret("Contract:\n");
    ret += _name->getString();
    ret += "\nDeclarations:\n";

    for(const auto & d : declarations)
    {
        ret += d->getString();
        ret += "\n";
    }

    ret += "Assumptions:\n";
    for(const auto & sit: assumptions)
    {
        ret += sit.second->getString();
        ret += "\n";
    }

    ret += "Guarantees:\n";
    for(const auto & sit: guarantees)
    {
        ret += sit.second->getString();
        ret += "\n";
    }

    ret += "=======================";
    return ret;
}

Name * Contract::getName() const {
    return _name.get();
}

void Contract::setName(Name * name)
{
    Declaration::setName(name);
}

void Contract::addAssumptions(semantic_domain domain, Specification *spec)
{
    auto it = assumptions.find(domain);
    if(it == assumptions.end())
        assumptions.emplace(domain, std::unique_ptr<Specification>(spec));
    else
        it->second.reset(spec);
    if(spec != nullptr) spec->setParent(this);
}

void Contract::addGuarantees(semantic_domain domain, Specification *spec)
{
    auto it = guarantees.find(domain);
    if(it == guarantees.end())
        guarantees.emplace(domain, std::unique_ptr<Specification>(spec));
    else
        it->second.reset(spec);
    if(spec != nullptr) spec->setParent(this);
}



/// \todo Implement the clone method.
Contract *Contract::clone() {
    auto ret = new Contract(_name->getString());

    // Corresponences maps.
    std::map< Declaration *, Declaration * > declaration_map;

    // Declarations.
    for(auto & it : declarations )
    {
        auto dec = it->clone();
        std::pair< Declaration *, Declaration * > p(it.get(), dec);
        ret->addDeclaration(dec);
        declaration_map.insert(p);
    }

    // Assumptions.
    for(auto & assumption : assumptions)
    {
        auto spec = assumption.second->clone();
        ret->addAssumptions(assumption.first, spec);
    }

    // Guarantees.
    for(auto & guarantee : guarantees)
    {
        auto spec = guarantee.second->clone();
        ret->addGuarantees(guarantee.first, spec);
    }

    ClonedDeclarationVisitor c(declaration_map);
    ret->accept_visitor(c);

    return ret;
}









