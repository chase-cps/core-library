/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include <utility>

#include "representation/Contract.hh"
#include "representation/Variable.hh"
#include "utilities/ClonedDeclarationVisitor.hh"
#include "utilities/Factory.hh"

using namespace chase;

void Contract:: mergeDeclarations(
        Contract * c1,
        Contract * c2,
        Contract * r,
        names_projection_map &correspondences,
        std::map< Declaration *, Declaration * >& declaration_map
        )
{
    /// \todo Implement the type checking.

    for(auto original : c1->declarations) {
        auto cloned = original->clone();
        std::pair< Declaration*, Declaration *> p(original, cloned);
        declaration_map.insert(p);

        r->declarations.push_back(cloned);
    }

    for(auto original : c2->declarations)
    {
        std::string name = original->getName()->getString();
        auto found = correspondences.find(name);

        if( found == correspondences.end())
        {
            // Check for name clashing between C1 and C2.
            for(auto & declaration : r->declarations) {
                if( name == declaration->getName()->getString() )
                    messageError("Name clashing in composition: " + name);
            }

            auto cloned = original->clone();
            std::pair< Declaration*, Declaration *> p(original, cloned);
            declaration_map.insert(p);

            r->declarations.push_back(cloned);
        }
        else {
            std::string to_find = found->second;
            for(auto & declaration : r->declarations) {
                if(declaration->getName()->getString() == to_find ) {
                    std::pair< Declaration*, Declaration *> p(original,declaration);
                    declaration_map.insert(p);
                }
            }
        }
    }
}


Contract * Contract::composition(
        Contract * c1, Contract * c2,
        names_projection_map & correspondences,
        std::string name)
{
    auto composed = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, composed, correspondences, declaration_map);

    // Fix variables causality.
    // If it is output in at least of the composing contracts, then it must be
    // output (i.e., controlled) variable.
    for(auto & dit : composed->declarations)
    {
        if(dit->IsA() == variable_node)
        {
            std::vector< Declaration * > originals;
            for(auto & mit : declaration_map )
            {
                if(mit.second == dit)
                    originals.push_back(mit.first);
            }
            for(auto & vit : originals)
            {
                if(vit->IsA() == variable_node)
                {
                    auto var = reinterpret_cast< Variable * >(dit);
                    auto original =
                            reinterpret_cast< Variable * >(vit);
                    if(original->getCausality() == output && var->getCausality() == input)
                    {
                        var->setCausality(output);
                    }
                }
            }
        }
    }

    composeLogic(c1, c2, composed);

    ClonedDeclarationVisitor v(declaration_map);
    composed->accept_visitor(v);

    return composed;
}

void Contract::composeLogic(
        Contract * c1,
        Contract * c2,
        Contract * r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(g1->clone(), g2->clone());
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = g2->clone();
    } else {
        guarantees = g1->clone();
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = And(a1->clone(), a2->clone());
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = a2->clone();
    } else {
        assumptions = a1->clone();
    }
    assumptions = Or(assumptions, Not(guarantees->clone()));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}

Contract * Contract::conjunction(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    conjoinLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}




void Contract::conjoinLogic(Contract *c1, Contract *c2, Contract *r)
{
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if( g1 != nullptr && g2 != nullptr ) {
        guarantees = And(g1->clone(), g2->clone());
    } else if(g1 == nullptr && g2 == nullptr) {
        guarantees = True();
    } else if(g1 == nullptr && g2 != nullptr) {
        guarantees = g2->clone();
    } else {
        guarantees = g1->clone();
    }

    if( a1 != nullptr && a2 != nullptr ) {
        assumptions = Or(a1->clone(), a2->clone());
    } else if(a1 == nullptr && a2 == nullptr) {
        assumptions = True();
    } else if(a1 == nullptr && a2 != nullptr) {
        assumptions = a2->clone();
    } else {
        assumptions = a1->clone();
    }


    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);


}

Contract * Contract::quotient(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name, bool synthesizable)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res,
                      correspondences, declaration_map);

    quotientLogic(c1, c2, res, synthesizable);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}



void Contract::quotientLogic(
        Contract *c1, Contract *c2, Contract *r, bool synthesizable) {
    // Quotient: C1 / C2 where C1 = (A1, G1) is the system contract,
    // C2 = (A2, G2) is the known sub-contract, both saturated.
    // Result: the largest contract C such that C2 ∥ C ⊑ C1.
    //
    // A_result = A1 ∧ G2
    // G_result = (G1 ∧ A2) ∨ ¬(A1 ∧ G2)
    //
    // With synthesizable set, the guarantees are G1 ∧ A2 only. The contract
    // is the same once saturated, but the formula is easier to synthesize.
    //
    // Reference: Incer et al., "Quotient for Assume-Guarantee Contracts",
    // MEMOCODE 2018, Theorem 3.5.

    LogicFormula *a1 = nullptr;
    LogicFormula *a2 = nullptr;
    LogicFormula *g1 = nullptr;
    LogicFormula *g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if (i != c1->assumptions.end()) {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if (a1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if (i != c2->assumptions.end()) {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if (a2 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if (i != c1->guarantees.end()) {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if (g1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if (i != c2->guarantees.end()) {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if (g2 == nullptr) messageError("Wrong format in Logic.");
    }

    // Assumptions: A1 ∧ G2
    LogicFormula *assumptions = nullptr;
    if (a1 != nullptr && g2 != nullptr)
        assumptions = And(a1->clone(), g2->clone());
    else if (a1 == nullptr && g2 == nullptr)
        assumptions = True();
    else if (a1 == nullptr)
        assumptions = g2->clone();
    else
        assumptions = a1->clone();

    // Guarantees: G1 ∧ A2
    LogicFormula *guarantees = nullptr;
    if (g1 != nullptr && a2 != nullptr)
        guarantees = And(g1->clone(), a2->clone());
    else if (g1 == nullptr && a2 == nullptr)
        guarantees = True();
    else if (g1 == nullptr)
        guarantees = a2->clone();
    else
        guarantees = g1->clone();

    // Saturation: (G1 ∧ A2) ∨ ¬(A1 ∧ G2)
    if (!synthesizable)
        guarantees = Or(guarantees, Not(assumptions->clone()));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);

}



Contract * Contract::separation(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res,
                      correspondences, declaration_map);

    separationLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}



void Contract::separationLogic(
        Contract *c1, Contract *c2, Contract *r) {
    // Separation: C1 ÷ C2, the adjoint of merging (•), dual of the quotient.
    // C1 = (A1, G1), C2 = (A2, G2) saturated (G includes ¬A).
    // Result: the smallest contract C such that C1 ⊑ C • C2, i.e.
    //     C1 ÷ C2 ⊑ C   iff   C1 ⊑ C • C2.
    //
    // Reference: Passerone, Incer, Sangiovanni-Vincentelli, "Coherent
    // extension, composition, and merging operators in contract models for
    // system design", ACM TECS 2019, Theorem 3.12.
    //
    // Derivation. The reciprocal C^-1 = (G, A) reverses refinement and maps
    // composition onto merging: (C ∥ C')^-1 = C^-1 • C'^-1. Hence
    //     C1 ÷ C2 = ( C1^-1 / C2^-1 )^-1.
    // With the quotient C / C' = (A ∧ G', (A' ∧ G) ∨ ¬(A ∧ G')) applied to
    // C1^-1 = (G1, A1) and C2^-1 = (G2, A2) one gets
    //     C1^-1 / C2^-1 = (G1 ∧ A2, (G2 ∧ A1) ∨ ¬(G1 ∧ A2)),
    // and taking the reciprocal:
    //
    // A_result = (A1 ∧ G2) ∨ ¬(G1 ∧ A2)
    // G_result = G1 ∧ A2
    //
    // The result is saturated: ¬A_result implies G1 ∧ A2.

    LogicFormula *a1 = nullptr;
    LogicFormula *a2 = nullptr;
    LogicFormula *g1 = nullptr;
    LogicFormula *g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if (i != c1->assumptions.end()) {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if (a1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if (i != c2->assumptions.end()) {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if (a2 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if (i != c1->guarantees.end()) {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if (g1 == nullptr) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if (i != c2->guarantees.end()) {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if (g2 == nullptr) messageError("Wrong format in Logic.");
    }

    // Guarantees: G1 ∧ A2
    LogicFormula *guarantees = nullptr;
    if (g1 != nullptr && a2 != nullptr)
        guarantees = And(g1->clone(), a2->clone());
    else if (g1 == nullptr && a2 == nullptr)
        guarantees = True();
    else if (g1 == nullptr)
        guarantees = a2->clone();
    else
        guarantees = g1->clone();

    // Assumptions: (A1 ∧ G2) ∨ ¬(G1 ∧ A2)
    LogicFormula *assumptions = nullptr;
    if (a1 != nullptr && g2 != nullptr)
        assumptions = And(a1->clone(), g2->clone());
    else if (a1 == nullptr && g2 == nullptr)
        assumptions = True();
    else if (a1 == nullptr)
        assumptions = g2->clone();
    else
        assumptions = a1->clone();
    assumptions = Or(assumptions, Not(guarantees->clone()));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}




Contract * Contract::disjunction(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    disjoinLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}

void Contract::disjoinLogic(Contract *c1, Contract *c2, Contract *r) {
    // Disjunction (least upper bound):
    // C1 ∨ C2 = (A1 ∧ A2, G1 ∨ G2)
    // Reference: Benveniste et al. (2009), Incer (2022).

    LogicFormula *a1 = nullptr, *a2 = nullptr;
    LogicFormula *g1 = nullptr, *g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if (i != c1->assumptions.end()) {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if (a1 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c2->assumptions.find(logic);
    if (i != c2->assumptions.end()) {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if (a2 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c1->guarantees.find(logic);
    if (i != c1->guarantees.end()) {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if (g1 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c2->guarantees.find(logic);
    if (i != c2->guarantees.end()) {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if (g2 == nullptr) messageError("Wrong format in Logic.");
    }

    // Assumptions: A1 ∧ A2
    LogicFormula *assumptions = nullptr;
    if (a1 != nullptr && a2 != nullptr)
        assumptions = And(a1->clone(), a2->clone());
    else if (a1 == nullptr && a2 == nullptr)
        assumptions = True();
    else if (a1 == nullptr)
        assumptions = a2->clone();
    else
        assumptions = a1->clone();

    // Guarantees: G1 ∨ G2
    LogicFormula *guarantees = nullptr;
    if (g1 != nullptr && g2 != nullptr)
        guarantees = Or(g1->clone(), g2->clone());
    else if (g1 == nullptr && g2 == nullptr)
        guarantees = True();
    else if (g1 == nullptr)
        guarantees = g2->clone();
    else
        guarantees = g1->clone();

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}


Contract * Contract::merging(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    mergingLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}

void Contract::mergingLogic(Contract *c1, Contract *c2, Contract *r) {
    // Merging (viewpoint fusion):
    // C1 ⊔ C2 = (A1 ∧ A2, G1 ∧ G2)
    // Reference: Passerone et al. (2019), Incer (2022).

    LogicFormula *a1 = nullptr, *a2 = nullptr;
    LogicFormula *g1 = nullptr, *g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if (i != c1->assumptions.end()) {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if (a1 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c2->assumptions.find(logic);
    if (i != c2->assumptions.end()) {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if (a2 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c1->guarantees.find(logic);
    if (i != c1->guarantees.end()) {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if (g1 == nullptr) messageError("Wrong format in Logic.");
    }
    i = c2->guarantees.find(logic);
    if (i != c2->guarantees.end()) {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if (g2 == nullptr) messageError("Wrong format in Logic.");
    }

    // Assumptions: A1 ∧ A2
    LogicFormula *assumptions = nullptr;
    if (a1 != nullptr && a2 != nullptr)
        assumptions = And(a1->clone(), a2->clone());
    else if (a1 == nullptr && a2 == nullptr)
        assumptions = True();
    else if (a1 == nullptr)
        assumptions = a2->clone();
    else
        assumptions = a1->clone();

    // Guarantees: G1 ∧ G2
    LogicFormula *guarantees = nullptr;
    if (g1 != nullptr && g2 != nullptr)
        guarantees = And(g1->clone(), g2->clone());
    else if (g1 == nullptr && g2 == nullptr)
        guarantees = True();
    else if (g1 == nullptr)
        guarantees = g2->clone();
    else
        guarantees = g1->clone();

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}


// The Logic part of assumptions or guarantees; an empty part is true.
static LogicFormula * logicPart(std::map< semantic_domain, Specification * > & m)
{
    auto i = m.find(logic);
    if (i == m.end()) return True();
    auto f = dynamic_cast<LogicFormula *>(i->second);
    if (f == nullptr) messageError("Wrong format in Logic.");
    return f->clone();
}


Contract * Contract::implication(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    implicationLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}

void Contract::implicationLogic(Contract *c1, Contract *c2, Contract *r) {
    // Implication: c1 → c2, the right adjoint of conjunction, i.e. the
    // largest contract C such that C ∧ c1 refines c2. With c1 = (A1, G1)
    // and c2 = (A2, G2) saturated:
    //
    // A_result = (A2 ∧ ¬A1) ∨ (G1 ∧ ¬G2)
    // G_result = G2 ∨ ¬G1
    //
    // Reference: Incer, Benveniste, Sangiovanni-Vincentelli, "Some
    // algebraic aspects of assume-guarantee reasoning", arXiv 2309.08875,
    // Proposition 6 (there written C' → C with C' = c1 and C = c2).

    auto a1 = logicPart(c1->assumptions);
    auto g1 = logicPart(c1->guarantees);
    auto a2 = logicPart(c2->assumptions);
    auto g2 = logicPart(c2->guarantees);

    auto assumptions = Or(And(a2, Not(a1)), And(g1->clone(), Not(g2->clone())));
    auto guarantees = Or(g2, Not(g1));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}


Contract * Contract::coimplication(
        Contract *c1, Contract *c2,
        names_projection_map &correspondences,
        std::string name)
{
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, res, correspondences, declaration_map);

    coimplicationLogic(c1, c2, res);

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}

void Contract::coimplicationLogic(Contract *c1, Contract *c2, Contract *r) {
    // Coimplication: c1 ⇝ c2, the left adjoint of disjunction and the dual
    // of the implication, i.e. the smallest contract C such that c2
    // refines C ∨ c1. With c1 = (A1, G1) and c2 = (A2, G2) saturated:
    //
    // A_result = A2 ∨ ¬A1
    // G_result = (G2 ∧ ¬G1) ∨ (A1 ∧ ¬A2)
    //
    // Reference: Incer, Benveniste, Sangiovanni-Vincentelli, arXiv
    // 2309.08875, Proposition 7 (there written C' ⇝ C with C' = c1 and
    // C = c2).

    auto a1 = logicPart(c1->assumptions);
    auto g1 = logicPart(c1->guarantees);
    auto a2 = logicPart(c2->assumptions);
    auto g2 = logicPart(c2->guarantees);

    auto assumptions = Or(a2->clone(), Not(a1->clone()));
    auto guarantees = Or(And(g2, Not(g1)), And(a1, Not(a2)));

    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}


Contract * Contract::reciprocal(Contract *c, std::string name)
{
    // Reciprocal: C^-1 = (G, A), the view of the environment. It reverses
    // refinement and exchanges composition with merging (Incer,
    // Benveniste, Sangiovanni-Vincentelli, arXiv 2309.08875, Section 2.1).
    // The inputs of the contract are the outputs of its environment.
    auto res = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    for(auto original : c->declarations) {
        auto cloned = original->clone();
        auto var = dynamic_cast<Variable *>(cloned);
        if(var != nullptr) {
            if(var->getCausality() == input) var->setCausality(output);
            else if(var->getCausality() == output) var->setCausality(input);
        }
        std::pair< Declaration*, Declaration *> p(original, cloned);
        declaration_map.insert(p);
        res->declarations.push_back(cloned);
    }

    res->addAssumptions(logic, logicPart(c->guarantees));
    res->addGuarantees(logic, logicPart(c->assumptions));

    ClonedDeclarationVisitor v(declaration_map);
    res->accept_visitor(v);

    return res;
}


void Contract::saturate(Contract *c)
{
    saturateLogic(c);
}

void Contract::saturateLogic(Contract * c )
{
    auto a = c->assumptions.find(logic);
    auto g = c->guarantees.find(logic);

    LogicFormula * assumptions = nullptr;
    LogicFormula * guarantees = nullptr;

    if(a != c->assumptions.end())
    {
        assumptions = dynamic_cast< LogicFormula * >(a->second);
        if(assumptions == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    if(g != c->guarantees.end())
    {
        guarantees = dynamic_cast< LogicFormula * >(g->second);
        if(guarantees == nullptr)
            messageError("Non logic formula in temporal logic domain");
    }

    if(assumptions == nullptr )
        return; // No saturation necessary.

    if( guarantees == nullptr )
        // Then, create new guarantees negating the
        // assumptions.
    {
        std::pair< semantic_domain, Specification * > p(
                logic, Not(assumptions->clone()));
        c->guarantees.insert(p);
    }
    else
    {
        guarantees = Or(Not(assumptions->clone()), guarantees);
        g->second = guarantees;
        guarantees->setParent(c);
    }
}


Contract *Contract::refinementCheck(Contract *c1, Contract *c2, names_projection_map &correspondences,
                               std::string name) {
    auto rcheck = new Contract(name);

    std::map< Declaration *, Declaration * > declaration_map;
    mergeDeclarations(c1, c2, rcheck, correspondences, declaration_map);

    /// \todo Check that variables have the same causality in both contracts.

    refinementCheckLogic(c1, c2, rcheck);

    ClonedDeclarationVisitor v(declaration_map);
    rcheck->accept_visitor(v);

    return rcheck;
}

void Contract::refinementCheckLogic(Contract *c1, Contract *c2, Contract *r) {
    LogicFormula * a1 = nullptr;
    LogicFormula * a2 = nullptr;
    LogicFormula * g1 = nullptr;
    LogicFormula * g2 = nullptr;

    auto i = c1->assumptions.find(logic);
    if(i != c1->assumptions.end())
    {
        a1 = dynamic_cast<LogicFormula *>(i->second);
        if( a1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->assumptions.find(logic);
    if(i != c2->assumptions.end())
    {
        a2 = dynamic_cast<LogicFormula *>(i->second);
        if( a2 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c1->guarantees.find(logic);
    if(i != c1->guarantees.end())
    {
        g1 = dynamic_cast<LogicFormula *>(i->second);
        if( g1 == nullptr ) messageError("Wrong format in Logic.");
    }

    i = c2->guarantees.find(logic);
    if(i != c2->guarantees.end())
    {
        g2 = dynamic_cast<LogicFormula *>(i->second);
        if( g2 == nullptr ) messageError("Wrong format in Logic.");
    }


    /// \todo Reason about correctness in the case one is a nullptr.
    // The formulas of c1 and c2 are cloned: the result owns its formulas.
    a1 = a1 == nullptr ? True() : a1->clone();
    a2 = a2 == nullptr ? True() : a2->clone();
    g1 = g1 == nullptr ? True() : g1->clone();
    g2 = g2 == nullptr ? True() : g2->clone();

    g1 = Or(g1, Not(a1->clone()));
    g2 = Or(g2, Not(a2->clone()));

    auto assumptions = Implies(a2, a1);
    auto guarantees = Implies(g1, g2);
    r->addAssumptions(logic, assumptions);
    r->addGuarantees(logic, guarantees);
}