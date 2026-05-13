/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/11/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/Value.hh"
#include "representation/Specification.hh"

#include <memory>

namespace chase {

    class ProbabilityFunction : public Value {
    public:
        /// @brief Constructor.
        /// @param specification Specification to evaluate. Ownership is transferred.
        ProbabilityFunction(Specification * specification = nullptr);

        /// @brief Destructor.
        ~ProbabilityFunction() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        ProbabilityFunction *clone() override;

        /// @brief Getter of the Specification.
        /// @return Pointer to the specification.
        Specification *getSpecification() const;

        /// @brief Setter of the specification.
        /// @param specification Pointer to th specification. Ownership is transferred.
        void setSpecification(Specification *specification);

        /// @brief Release ownership of the specification.
        /// @return The specification as an owning pointer.
        std::unique_ptr<Specification> releaseSpecification();

    protected:
        /// @brief The specification being evaluated.
        std::unique_ptr<Specification> _specification{};

    };

}
