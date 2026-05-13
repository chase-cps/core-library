/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        4/27/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "utilities/LogicSimplificationVisitor.hh"
#include "representation.hh"
#include "Factory.hh"
#include <memory>

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
        auto spec = it->second.release();
        auto formula = static_cast< LogicFormula * >(spec);
        if(formula != nullptr){
            it->second.reset(_analyzeFormula(formula));
            it->second->setParent(&contract);
        } else {
            it->second.reset(spec);
        }
    }

    it = contract.guarantees.find(logic);
    if(it != contract.guarantees.end()){
        auto spec = it->second.release();
        auto formula = static_cast< LogicFormula * >(spec);
        if(formula != nullptr){
            formula->accept_visitor(*this);
            it->second.reset(_analyzeFormula(formula));
            it->second->setParent(&contract);
        } else {
            it->second.reset(spec);
        }
    }

    return rv;
}

int LogicSimplificationVisitor::visitBinaryBooleanOperation(
        BinaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    auto op1 = formula.releaseOp1();
    formula.setOp1(_analyzeFormula(op1.release()));
    auto op2 = formula.releaseOp2();
    formula.setOp2(_analyzeFormula(op2.release()));

    rv |= formula.getOp1()->accept_visitor(*this);
    rv |= formula.getOp2()->accept_visitor(*this);

    return rv;
}

int LogicSimplificationVisitor::visitUnaryBooleanOperation(
        UnaryBooleanFormula &formula)
{
    int rv = formula.getOp1()->accept_visitor(*this);
    auto op1 = formula.releaseOp1();

    formula.setOp1(_analyzeFormula(op1.release()));

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
        auto op = formula.releaseOperand(i);
        formula.setOperand(i, _analyzeFormula(op.release()));
        rv |= formula.operands[i]->accept_visitor(*this);
    }
    return rv;
}

int LogicSimplificationVisitor::visitUnaryTemporalOperation(
        UnaryTemporalFormula &formula)
{
    int rv =formula.getFormula()->accept_visitor(*this);
    auto op1 = formula.releaseFormula();
    formula.setFormula(_analyzeFormula(op1.release()));
    rv |= formula.getFormula()->accept_visitor(*this);

    return rv;
}

int LogicSimplificationVisitor::visitBinaryTemporalOperation(
        BinaryTemporalFormula &formula)
{
    int rv = formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    auto op1 = formula.releaseFormula1();
    formula.setFormula1(_analyzeFormula(op1.release()));
    auto op2 = formula.releaseFormula2();
    formula.setFormula2(_analyzeFormula(op2.release()));

    rv |= formula.getFormula1()->accept_visitor(*this);
    rv |= formula.getFormula2()->accept_visitor(*this);

    return rv;
}

LogicFormula * LogicSimplificationVisitor::_analyzeFormula(
    LogicFormula * formula ){
    std::unique_ptr<LogicFormula> ownedFormula(formula);
    if(formula->IsA() == largeBooleanFormula_node)
    {
        auto lbf = static_cast< LargeBooleanFormula * >(formula);
        if(lbf->operands.size() == 1) {
            auto ret = lbf->releaseOperand(0);
            return ret.release();
        }
        if(lbf->operands.size() == 2){
            auto op1 = lbf->releaseOperand(0);
            auto op2 = lbf->releaseOperand(1);
            return And(op1.release(), op2.release());
        }
    }
    return ownedFormula.release();
}
