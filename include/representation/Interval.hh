/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/27/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "representation/IntegerValue.hh"
#include "representation/RealValue.hh"

#include <memory>

namespace chase {

    class Interval : public Value {
    public:
        /// @brief Constructor.
        /// @brief lbound Lower bound of the interval. Ownership is transferred.
        /// @brief rbound Upper bound of the interval. Ownership is transferred.
        /// @brief leftOpen flag set to true if the interval is to be left open.
        /// @brief rightOpen flag se to true if the interval is to be right open.
        explicit Interval(
                Value * lbound = nullptr,
                Value * rbound = nullptr,
                bool leftOpen = false,
                bool rightOpen = false
                );

        /// @brief Function to access the left bound value.
        /// @return a pointer to the left bound value.
        Value *getLeftBound() const;
        /// @brief Function to access the right bound value.
        /// @return a pointer to the right bound value.
        Value *getRightBound() const;

        /// @brief Function to know whether the interval is left open or not.
        /// @return True if the interval is left open. False otherwise.
        bool isLeftOpen() const;

        /// @brief Function to know whether the interval is right open or not.
        /// @return True if the interval is right open. False otherwise.
        bool isRightOpen() const;

        /// @brief Destructor.
        ~Interval() override;

        /// @brief Return the type of the interval.
        /// @return Pointer to the type of the interval. I.e., the type of
        /// the most precise bound.
        Type *getType() override;

        /// @brief Method to clone the interval.
        /// @return a (deep) Clone of the interval.
        Interval *clone() override;

        /// @brief Base method to visit the interval.
        /// @return The value returned by the visit. It depends on the visitor.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Method to print the interval.
        /// @reutrn The string corresponding to the visitor.
        std::string getString() override;

        /// @brief Release ownership of the left bound.
        /// @return The left bound as an owning pointer.
        std::unique_ptr<Value> releaseLeftBound();

        /// @brief Release ownership of the right bound.
        /// @return The right bound as an owning pointer.
        std::unique_ptr<Value> releaseRightBound();

    protected:

        /// @brief Left bound of the interval.
        std::unique_ptr<Value> _leftBound;
        /// @brief Right bound of the interval.
        std::unique_ptr<Value> _rightBound;

        /// @brief Flag set to true to indicate if the interval is left open.
        bool _leftOpen;
        /// @brief Flag set to true to indicate if the interval is right open.
        bool _rightOpen;

    };

}
