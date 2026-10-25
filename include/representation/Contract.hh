/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation/ChaseObject.hh"
#include "Specification.hh"
#include "Scope.hh"

#include <list>
#include <map>



namespace chase {

    /// @brief Data structure used to store the projections of the names to be
    /// used to during the operations in the algebra.
    typedef std::map<std::string, std::string> names_projection_map;

    /// @brief Class to represent contracts.
    class Contract : public Scope {
    public:

        /// @brief List of assumptions.
        std::map< semantic_domain, Specification * > assumptions;

        /// @brief List of guarantees.
        std::map< semantic_domain, Specification * > guarantees;

        /// @brief Constructor.
        explicit Contract(std::string name = std::string("contract"));

        /// @brief Destructor.
        ~Contract() override;

        /// @brief Getter of the contract's name.
        /// @return The contract name.
        Name * getName() const;

        /// @brief Setter of the contract's name.
        /// @param name The contract name.
        void setName(Name *name) override;

        /// @brief Function to safely add the assumptions for a given semantic
        /// domain. It takes care of managing the parent link to the contract.
        /// @param domain The semantic domain.
        /// @param spec Pointer to the the specification to add.
        void addAssumptions( semantic_domain domain, Specification * spec );
        /// @brief Function to safely add the guarantees for a given semantic
        /// domain. It takes care of managing the parent link to the contract.
        /// @param domain The semantic domain.
        /// @param spec Pointer to the the specification to add.
        void addGuarantees( semantic_domain domain, Specification * spec );

        /// @brief Base function for the visitor pattern.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function printing the contract into a string.
        /// @return The string representing the contract.
        std::string getString() override;

        /// @brief Clone method.
        /// @return A clone of the contract.
        Contract * clone() override;


        // -- Methods for the Contract Algebra.

        /// @brief Method to saturate a contract. Note: the contract is
        /// saturated in loco. No copies are produced.
        /// @param c Contract to saturate.
        static void saturate( Contract * c );

        /// @brief Method implementing the composition of two contracts.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the composition of the
        /// two.
        static Contract * composition(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("composition"));



        /// @brief Function performing composition for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void composeLogic(
                Contract *c1, Contract *c2, Contract * r);


        static Contract * refinementCheck(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("refinement")
        );

        static void refinementCheckLogic(
                Contract *c1, Contract *c2, Contract * r
                );

        /// @brief Method implementing the composition of two contracts.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the composition of the
        /// two.
        static Contract * conjunction(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("conjunction"));

        /// @brief Function performing composition for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void conjoinLogic(
                Contract *c1, Contract *c2, Contract * r);

        /// @brief Method implementing the disjunction of two contracts.
        /// Disjunction is the least upper bound in the contract lattice:
        /// C1 ∨ C2 = (A1 ∧ A2, G1 ∨ G2).
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the disjunction of the
        /// two.
        static Contract * disjunction(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("disjunction"));

        /// @brief Function performing disjunction for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void disjoinLogic(
                Contract *c1, Contract *c2, Contract * r);

        /// @brief Method implementing the merging of two contracts.
        /// Merging is the viewpoint fusion operation:
        /// C1 ⊔ C2 = (A1 ∧ A2, G1 ∧ G2).
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the merging of the
        /// two.
        static Contract * merging(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("merging"));

        /// @brief Function performing merging for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void mergingLogic(
                Contract *c1, Contract *c2, Contract * r);

        /// @brief Method implementing the quotient of two contracts.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the quotient of the
        /// two.
        static Contract * quotient(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("quotient"),
                bool synthesizable=false);

        /// @brief Function performing Quotient for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void quotientLogic(
                Contract *c1, Contract *c2, Contract * r,
                bool synthesizable=false);

        /// @brief Method implementing the separation of two contracts.
        /// Separation is the adjoint of merging and the dual of the
        /// quotient: C1 ÷ C2 = (C1^-1 / C2^-1)^-1, where C^-1 = (G, A).
        /// For saturated contracts:
        /// C1 ÷ C2 = ((A1 ∧ G2) ∨ ¬(G1 ∧ A2), G1 ∧ A2).
        /// It is the smallest contract C such that C1 refines C merged
        /// with C2.
        /// @param c1 The first contract (the contract to separate).
        /// @param c2 The second contract (the known viewpoint).
        /// @param correspondences Map of the correspondences between variables
        /// in the two contracts being composed. Each entry in the map has key
        /// the names in the second contract, and the value is the name of the
        /// projected variable in the first contract.
        /// This choice improve computational complexity, as the variables of
        /// the first contract are used as base for the composed contract.
        /// @param name The name of the resulting contract.
        /// @return Pointer to a new contract that is the separation of the
        /// two.
        static Contract * separation(
                Contract * c1, Contract * c2,
                names_projection_map & correspondences,
                std::string name = std::string("separation"));

        /// @brief Function performing separation for Logic specifications.
        /// @param c1 The first contract.
        /// @param c2 The second contract.
        /// @param r The resulting contract.
        static void separationLogic(
                Contract *c1, Contract *c2, Contract * r);

        /// @brief Method merging the declarations of two contracts into one. It
        /// is used by the operations to create the declarations of the
        /// resulting contract when applying operations over two contracts.
        /// @param c1 First contract which declarations has to be merged.
        /// @param c2 Second contract which declarations has to be merged.
        /// @param r The resulting contract (pointer to). The contract which
        /// declarations will be the merging of.
        /// @param correspondences Map of the correspondences of variable
        /// names.
        static void mergeDeclarations(
                Contract * c1,
                Contract * c2,
                Contract * r,
                names_projection_map & correspondences,
                std::map< Declaration *, Declaration * >& declaration_map);

        /// @brief Method implementing the saturation for the temporal logic
        /// specifications in contracts.
        /// @param c The contract to saturate. Saturation happens in loco.
        static void saturateLogic(Contract * c );

    protected:

        /// @brief Name of the contract. The default value will be "contract".
        /// It will be useful to identify a contract within a system modeled
        /// by multiple contracts.
        Name * _name;





    };

}
