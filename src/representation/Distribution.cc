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
        DataDeclaration(type, name),
        _distribution_type(dtype)
{
    _node_type = distribution_node;
}

Distribution::~Distribution() {
    delete(_name);
    delete(_type);
    parameters.clear();
}

distribution_type Distribution::getDistributionType() const {
    return _distribution_type;
}

void Distribution::setDistributionType(distribution_type distributionType) {
    _distribution_type = distributionType;
}

Value * Distribution::parameter(std::string name, chase::Value * value)
{
    if(value != nullptr) {
        std::pair< std::string, Value * > p(name, value);
        parameters.insert(p);
        return value;
    } else {
        auto it = parameters.find(name);
        if(it != parameters.end()){
            return it->second;
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
    for(auto it: parameters)
        ret += "\t" + it.first + " = " + it.second->getString();
    ret += "\t),\n";
    ret += "\t" + _type->getString() + "\n";
    return ret;
}

Distribution * Distribution::clone() {
    auto distribution = new Distribution(
            _distribution_type, _name->clone(), _type->clone());
    for(auto it :   parameters)
        distribution->parameters.insert(
                std::pair<std::string, chase::Value*>(
                        it.first, it.second->clone()));
    return distribution;
}

