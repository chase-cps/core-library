/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "utilities/LogicSimplificationVisitor.hh"
#include "representation.hh"
#include "Factory.hh"

using namespace chase;

LogicSimplificationVisitor::LogicSimplificationVisitor() {
}

LogicSimplificationVisitor::~LogicSimplificationVisitor() {
}


int LogicSimplificationVisitor::visitContract(Contract &contract)
{
    int rv = 0;

    auto it = contract.assumptions.find(logic);
    if(it != contract.assumptions.end()){
        auto spec = it->second;
        auto formula = static_cast< LogicFormula * >(spec);
        if(formula != nullptr){
            it->second = _analyzeFormula(formula);
        }
    }

    it = contract.guarantees.find(logic);
    if(it != contract.guarantees.end()){
        auto spec = it->second;
        auto formula = static_cast< LogicFormula * >(spec);
        if(formula != nullptr){
            formula->accept_visitor(*this);
            it->second = _analyzeFormula(formula);
        }
    }

    return rv;
}

int LogicSimplificationVisitor::visitBinaryBooleanOperation(
        BinaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    auto op1 = formula.getOp1();
    formula.setOp1(_analyzeFormula(op1));
    auto op2 = formula.getOp2();
    formula.setOp2(_analyzeFormula(op2));

    rv |= formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    return rv;
}

int LogicSimplificationVisitor::visitUnaryBooleanOperation(
        UnaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    auto op1 = formula.getOp1();

    formula.setOp1(_analyzeFormula(op1));

    rv |= formula.getOp1()->accept_visitor(*this);
    return rv;
}

int LogicSimplificationVisitor::visitLargeBooleanFormula(
        LargeBooleanFormula &formula)
{
    int rv = 0;
    for(size_t i = 0; i < formula.operands.size(); ++i)
    {
        rv |= formula.operands[i]->accept_visitor(*this);
        auto op = formula.operands[i];
        formula.operands[i] = _analyzeFormula(op);
        rv |= formula.operands[i]->accept_visitor(*this);
    }
    return rv;
}

int LogicSimplificationVisitor::visitUnaryTemporalOperation(
        UnaryTemporalFormula &formula)
{
    int rv =formula.getFormula()->accept_visitor(*this);
    auto op1 = formula.getFormula();
    formula.setFormula(_analyzeFormula(op1));
    rv |= formula.getFormula()->accept_visitor(*this);

    return rv;
}

int LogicSimplificationVisitor::visitBinaryTemporalOperation(
        BinaryTemporalFormula &formula)
{
    int rv = formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    auto op1 = formula.getFormula1();
    formula.setFormula1(_analyzeFormula(op1));
    auto op2 = formula.getFormula2();
    formula.setFormula2(_analyzeFormula(op2));

    rv |= formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    return rv;
}

LogicFormula * LogicSimplificationVisitor::_analyzeFormula(
    LogicFormula * formula ){
    if(formula->IsA() == largeBooleanFormula_node)
    {
        auto lbf = static_cast< LargeBooleanFormula * >(formula);
        if(lbf->operands.size() == 1) {
            return lbf->operands[0];
        }
        if(lbf->operands.size() == 2){
            return And(lbf->operands[0], lbf->operands[1]);
        }
    }
    return formula;
}
