/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/16/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "NuSMVPrinter.hh"

using namespace chase;

NuSMVPrinter::NuSMVPrinter( std::string path ) :
    GuideVisitor(),
    _contract(nullptr)
{
    _fout.open(path);
}

NuSMVPrinter::~NuSMVPrinter()
{
    if(_fout.is_open()) _fout.close();
}

void NuSMVPrinter::print(Contract *contract)
{
    _contract = contract;
    printDeclarations();
    printAssumptions();
    printGuarantees();
}

void NuSMVPrinter::printDeclarations() {
    _fout << "MODULE main" << std::endl;
    _fout << "VAR" << std::endl;
    for (auto & declaration : _contract->declarations)
    {
        /// \todo Support for constants.
        if( declaration->IsA() != variable_node ) continue;
        auto var = reinterpret_cast<Variable*>(declaration);
        var->accept_visitor(*this);
    }
}

void NuSMVPrinter::printAssumptions()
{
    _fout << "\tLTLSPEC (" << std::endl << "\t\t";
    auto it = _contract->assumptions.find(logic);
    // No assumptions. Surely true.
    if(it == _contract->assumptions.end()) {
        _fout << "\t\tTRUE" << std::endl;
        return;
    }
    // Else...
    it->second->accept_visitor(*this);
    _fout << " )\n\n";
}

void NuSMVPrinter::printGuarantees()
{
    _fout << "\tLTLSPEC (" << std::endl << "\t\t";
    auto it = _contract->guarantees.find(logic);
    // No assumptions. Surely true.
    if(it == _contract->guarantees.end()) {
        _fout << "\t\tTRUE" << std::endl;
        return;
    }
    // Else...
    it->second->accept_visitor(*this);
    _fout << " )\n\n";
}

std::string NuSMVPrinter::_printBooleanOperator(BooleanOperator op) {
    std::string sop;
    switch(op)
    {
        case op_and:        return "&";
        case op_or:         return "|";
        case op_implies:    return "->";
        case op_iff:        return "<->";
        case op_xor:        return "xor";
        case op_xnor:       return "xnor";
        case op_not:        return "!";
        case op_nand:
            messageError("Unsupported operator: NAND.");
            break;
        case op_nor:
            messageError("Unsupported operator: NOR.");
            break;
    }
    return std::string();
}

std::string NuSMVPrinter::_printTemporalOperator(TemporalOperator op) {
    std::string sop;
    switch(op)
    {
        case op_globally:   return "G";
        case op_future:     return "F";
        case op_next:       return "X";
        case op_until:      return "U";
        case op_release:
            messageError("Unsupported operator: Release");
            break;
    }
    return std::string();
}
