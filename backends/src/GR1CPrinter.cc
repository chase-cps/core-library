/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/3/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "GR1CPrinter.hh"

using namespace chase;

GR1CPrinter::GR1CPrinter() :
    GuideVisitor(),
    _inNext(false)
{
}

GR1CPrinter::~GR1CPrinter()
{
    if(fout.is_open())
        fout.close();
}

void GR1CPrinter::print(Contract *contract, std::string path)
{
    // Open the file and get the contract to be printed.
    _contract = contract;
    fout.open(path);

    // Print the declarations.
    _printDeclarations();

    _printInit();
    _printSafety();
    _printLiveness();

}



void GR1CPrinter::_printDeclarations()
{
    // Print the environment variables.
    fout << "ENV:" << std::endl;
    for( auto vit = _contract->declarations.begin();
        vit != _contract->declarations.end(); ++vit)
    {
        if( (*vit)->IsA() != variable_node ) continue;
        auto var = reinterpret_cast<Variable *>(*vit);
        if( var->getCausality() == input )
        {
            std::string name = var->getName()->getString();
            Type * type = var->getType();
            fout << "\t" << name;
            if(type->IsA() == integer_node)
            {
                auto integ = reinterpret_cast<Integer *>(type);
                auto lv = integ->getMin();
                auto rv = integ->getMax();

                fout << "[" << lv << ", " << rv << "]";
            }
            fout << std::endl;
        }
    }
    fout << ";" << std::endl << std::endl;


    // Print the system variables.
    fout << "SYS:" << std::endl;
    for( auto vit = _contract->declarations.begin();
         vit != _contract->declarations.end(); ++vit)
    {
        if( (*vit)->IsA() != variable_node ) continue;
        auto var = reinterpret_cast<Variable *>(*vit);
        if( var->getCausality() == output )
        {
            std::string name = var->getName()->getString();
            Type * type = var->getType();
            fout << "\t" << name;
            if(type->IsA() == integer_node)
            {
                auto integ = reinterpret_cast<Integer *>(type);
                auto lv = integ->getMin();
                auto rv = integ->getMax();

                fout << "[" << lv << ", " << rv << "]";
            }
            fout << std::endl;
        }
    }

    fout << ";" << std::endl << std::endl;

}



void GR1CPrinter::_printInit() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");


        _curr = std::string("");
        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {

            if( lbf->operands[f]->IsA() != unaryTemporalOperation_node &&
                    lbf->operands[f]->IsA() != binaryTemporalOperation_node )
            {
                if(_curr != "")
                    _curr += "\n\t& ";
                else
                    _curr += "\n\t";

                lbf->operands[f]->accept_visitor(*this);

            }
        }
        if(_curr != "")
            fout<< "ENVINIT:" << _curr
                << std::endl << ";" << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() != unaryTemporalOperation_node &&
                lbf->operands[f]->IsA() != binaryTemporalOperation_node )
            {
                if(_curr != "")
                    _curr += "\n\t& ";
                else
                    _curr += "\n\t";
                lbf->operands[f]->accept_visitor(*this);
            }

        }
        if(_curr != "")
            fout<< "SYSINIT:" << _curr
                << std::endl << ";" << std::endl << std::endl;
    }
}

void GR1CPrinter::_printSafety() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() == unaryTemporalOperation_node )
            {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula*>(lbf->operands[f]);
                if(uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if(uto->getFormula()->IsA() == unaryTemporalOperation_node)
                {
                    auto inner = reinterpret_cast<UnaryTemporalFormula*>(
                            uto->getFormula());
                    if( inner->getOp() == op_future )
                        continue; // Is a Liveness Property.
                }
                if(_curr != "")
                    _curr += "\n\t& ";
                else
                    _curr += "\n\t";
                uto->accept_visitor(*this);
            }
        }

        if(_curr != "")
            fout<< "ENVTRANS:"
                << _curr << std::endl  << ";" << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if (lbf->getOp() != op_and)
            messageError("Not a GR1 Specification.");

        _curr = std::string("");
        for (size_t f = 0; f < lbf->operands.size(); ++f) {
            if (lbf->operands[f]->IsA() == unaryTemporalOperation_node) {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula *>(lbf->operands[f]);
                if (uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if (uto->getFormula()->IsA() == unaryTemporalOperation_node) {
                    auto inner = reinterpret_cast<UnaryTemporalFormula *>(
                            uto->getFormula());
                    if (inner->getOp() == op_future)
                        continue; // Is a Liveness Property.
                }
                if(_curr != "")
                    _curr += "\n\t& ";
                else
                    _curr += "\n\t";
                uto->accept_visitor(*this);
            }
        }
        if (_curr != "")
            fout << "SYSTRANS:"
                 << _curr << std::endl  << ";" << std::endl << std::endl;
    }
}

void GR1CPrinter::_printLiveness() {
    // Assumptions.
    auto formulae = _contract->assumptions.find(logic);
    if( formulae == _contract->assumptions.end() )
        messageError("No Logic in Assumptions");

    auto assumptions = formulae->second;

    if( assumptions->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(assumptions);
        if (lbf->getOp() != op_and)
            messageError("Not a GR1 Specification.");

        _curr = std::string("");

        for (size_t f = 0; f < lbf->operands.size(); ++f) {
            if (lbf->operands[f]->IsA() == unaryTemporalOperation_node) {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula *>(lbf->operands[f]);
                if (uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if (uto->getFormula()->IsA() == unaryTemporalOperation_node) {
                    auto inner = reinterpret_cast<UnaryTemporalFormula *>(
                            uto->getFormula());
                    if (inner->getOp() == op_future) {
                        if(_curr != "")
                            _curr += "\n\t& ";
                        else
                            _curr += "\n\t";
                       uto->accept_visitor(*this);
                    }
                }
            }
        }
        if (_curr != "")
            fout << "ENVGOAL:"
                 << _curr << std::endl << ";" << std::endl << std::endl;
    }

    // Guarantees
    formulae = _contract->guarantees.find(logic);
    if( formulae == _contract->guarantees.end() )
        messageError("No Logic in Assumptions");

    auto guarantees = formulae->second;

    if( guarantees->IsA() == large_boolean_formula_node ) {
        auto lbf = reinterpret_cast< LargeBooleanFormula * >(guarantees);
        if( lbf->getOp() != op_and )
            messageError("Not a GR1 Specification.");


        _curr = std::string("");

        for( size_t f = 0; f < lbf->operands.size(); ++f )
        {
            if( lbf->operands[f]->IsA() == unaryTemporalOperation_node )
            {
                auto uto =
                        reinterpret_cast<UnaryTemporalFormula*>(lbf->operands[f]);
                if(uto->getOp() != op_globally)
                    messageError("Not a GR1 Specification");

                if(uto->getFormula()->IsA() == unaryTemporalOperation_node)
                {
                    auto inner = reinterpret_cast<UnaryTemporalFormula*>(
                            uto->getFormula());
                    if( inner->getOp() == op_future ) {
                        if(_curr != "")
                            _curr += "\n\t& ";
                        else
                            _curr += "\n\t";
                        uto->accept_visitor(*this);
                    }
                }

            }
        }
        if (_curr != "")
            fout << "SYSGOAL:"
                  << _curr << std::endl << ";" << std::endl << std::endl;
    }
}
