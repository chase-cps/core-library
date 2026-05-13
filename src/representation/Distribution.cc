/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/16/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "representation/Distribution.hh"
#include "representation/Value.hh"

using namespace chase;

Distribution::Distribution(
        distribution_type dtype, Name * name, Type * type) :
        DataDeclaration(type != nullptr ? type : reinterpret_cast<Type*>(new Real()),
                        name != nullptr ? name : new Name("anonymous_distribution")),
        _distribution_type(dtype)
{
    _node_type = distribution_node;
}

Distribution::~Distribution() = default;

distribution_type Distribution::getDistributionType() const {
    return _distribution_type;
}

void Distribution::setDistributionType(distribution_type distributionType) {
    _distribution_type = distributionType;
}

Value * Distribution::parameter(std::string name, chase::Value * value)
{
    if(value != nullptr) {
        auto it = parameters.find(name);
        if(it == parameters.end())
            parameters.emplace(name, std::unique_ptr<Value>(value));
        else
            it->second.reset(value);
        value->setParent(this);
        return value;
    } else {
        auto it = parameters.find(name);
        if(it != parameters.end()){
            return it->second.get();
        } else {
            return nullptr;
        }
    }
}

int Distribution::accept_visitor(BaseVisitor &v) {
    return v.visitDistribution(*this);
}

std::string Distribution::getString()
{
    std::string ret("distribution ");
    ret += _name->getString() + std::string(" is \n");

    ret += "\t" + distribution_type_names[_distribution_type];
    for(auto & it: parameters)
        ret += "\t" + it.first + " = " + it.second->getString();
    ret += "\t),\n";
    ret += "\t" + _type->getString() + "\n";
    return ret;
}

Distribution * Distribution::clone() {
    auto distribution = new Distribution(
            _distribution_type, _name->clone(), _type->clone());
    for(auto & it : parameters)
        distribution->parameter(it.first, it.second->clone());
    return distribution;
}

