/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/3/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/FunctionCall.hh"
#include "representation/Function.hh"

#include "utilities/IOUtils.hh"

using namespace chase;

FunctionCall::FunctionCall() :
    Value()
{
    _node_type = functionCall_node;
}

FunctionCall::~FunctionCall() = default;

int FunctionCall::accept_visitor(BaseVisitor &v) {
    return v.visitFunctionCall(*this);
}

std::string FunctionCall::getString() {
    std::string ret("");
    return ret;
}

FunctionCall *FunctionCall::clone() {
    auto ret = new FunctionCall();
    ret->setFunction(_function, false);
    ret->_parameters.clear();
    ret->_parameters.reserve(_parameters.size());
    for(auto & parameter : _parameters)
        ret->_parameters.emplace_back(
                parameter != nullptr ? parameter->clone() : nullptr);
    return ret;
}

Function *FunctionCall::getFunction() const {
    return _function;
}

void FunctionCall::setFunction(Function *function, bool initialize) {
    _function = function;
    if(function != nullptr && (initialize || _parameters.empty()))
    {
        _parameters.clear();
        _parameters.resize(function->getArity());
    }
}

Value *FunctionCall::parameter(size_t i, Value *value) {
    if(_function == nullptr || i >= _parameters.size()) return nullptr;
    if(value != nullptr)
    {
        if(i >= _function->getArity()) {
            messageWarning("The index is greater the function arity.");
            return nullptr;
        }
        _parameters[i].reset(value);
        value->setParent(this);
    }
    return _parameters[i].get();
}
