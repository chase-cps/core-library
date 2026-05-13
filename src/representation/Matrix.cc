/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/15/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/Matrix.hh"
#include "representation/Identifier.hh"
#include "representation/Integer.hh"
#include "representation/Real.hh"
#include "representation/Boolean.hh"

#include "utilities/IOUtils.hh"

using namespace chase;

Matrix::Matrix(
        unsigned int rows,
        unsigned int columns,
        std::vector< Value * > M) :
    _rows(rows),
    _columns(columns) {

    _node_type = matrix_node;

    elements.resize(_rows * columns);
    if(M.size() > 0 && M.size() != (rows * columns))
    {
        messageError("Mismatch in Matrix size and elements");
    }

    for (size_t it = 0; it < M.size(); ++it) {
        elements[it].reset(M[it]);
        if(elements[it] != nullptr) {
            _evaluateType(elements[it].get());
            elements[it]->setParent(this);
        }
    }
}

Matrix::~Matrix() = default;

unsigned int Matrix::getRows() const {
    return _rows;
}

unsigned int Matrix::getColumns() const {
    return _columns;
}

Value *Matrix::at(unsigned int i, unsigned int j, Value *value) {
    if(i >= 1 && j >= 1 && i <= _rows && j <= _columns) {
        unsigned int _i = i - 1;
        unsigned int _j = j - 1;
        size_t index = _i * _columns + _j;
        if (value != nullptr){
            // Check whether the type of the value is legal.
            Type * t = value->getType();
            switch(t->IsA())
            {
                case integer_node:
                case boolean_node:
                case real_node:
                    break;
                default:
                    messageError(
                            "Invalid type for scalar value:\n\t"
                            + t->getString());
            }
            elements[index].reset(value);
            _evaluateType(elements[index].get());
            elements[index]->setParent(this);
        }
        return elements[index].get();
    } else return nullptr;
}

int Matrix::accept_visitor(BaseVisitor &v) {
    return v.visitMatrix(*this);
}

std::string Matrix::getString() {
    std::string ret("\t[");
    for(size_t i = 1; i < _rows; ++i)
    {
        for (size_t j = 1; j < _columns; ++j) {
            auto value = at(i, j);
            if(value != nullptr) ret += value->getString();
            ret += ", ";
        }
        auto last = at(i, _columns);
        if(last != nullptr) ret += last->getString();
        ret += ";\n\t";
    }
    for (size_t j = 1; j < _columns; ++j) {
        auto value = at(_rows, j);
        if(value != nullptr) ret += value->getString();
        ret += ", ";
    }
    auto last = at(_rows, _columns);
    if(last != nullptr) ret += last->getString();
    ret += "]";
    return ret;
}

Matrix *Matrix::clone()
{
    std::vector<Value *> clonedElements;
    clonedElements.reserve(elements.size());
    for(auto & element : elements)
        clonedElements.push_back(element != nullptr ? element->clone() : nullptr);
    return new Matrix(_rows, _columns, clonedElements);
}

void Matrix::_evaluateType(Value *v)
{
    Type * t = v->getType();
    if(_type == nullptr) {
        setType(t->clone());
        return;
    }
    if(_type->IsA() == boolean_node && t->IsA() == integer_node) {
        setType(new Integer());
    }
    else if(_type->IsA() == boolean_node && t->IsA() == real_node)
    {
        setType(new Real());
    }
    else if(_type->IsA() == integer_node && t->IsA() == real_node)
    {
        setType(new Real());
    }
}
