#pragma once
#include "relation.hpp"
#include "../polynomials/univariate.hpp"

namespace proof_system::honk::sumcheck {

template <typename FF> class GrandProductInitializationRelation {
  public:
    constexpr bool scale_by_random_polynomial() { return true; }

    // 1 + polynomial degree of this relation
    static constexpr size_t RELATION_LENGTH = 3;

    /**
     * @brief Add contribution of the permutation relation for a given edge
     *
     * @details There are 2 relations associated with enforcing the wire copy relations
     * This file handles the relation Z_perm_shift(n_last) = 0 via the relation:
     *
     *                      C(X) = L_LAST(X) * Z_perm_shift(X)
     *
     * @param evals transformed to `evals + C(extended_edges(X)...)*scaling_factor`
     * @param extended_edges an std::array containing the fully extended Univariate edges.
     * @param parameters contains beta, gamma, and public_input_delta, ....
     * @param scaling_factor optional term to scale the evaluation before adding to evals.
     */
    void add_edge_contribution(Univariate<FF, RELATION_LENGTH>& evals,
                               const auto& extended_edges,
                               const RelationParameters<FF>&,
                               const FF& scaling_factor) const
    {
        auto z_perm_shift = UnivariateView<FF, RELATION_LENGTH>(extended_edges.z_perm_shift);
        auto lagrange_last = UnivariateView<FF, RELATION_LENGTH>(extended_edges.lagrange_last);

        evals += (lagrange_last * z_perm_shift) * scaling_factor;
    };

    void add_full_relation_value_contribution(FF& full_honk_relation_value,
                                              auto& purported_evaluations,
                                              const RelationParameters<FF>&,
                                              const FF& scaling_factor) const
    {
        auto z_perm_shift = purported_evaluations.z_perm_shift;
        auto lagrange_last = purported_evaluations.lagrange_last;

        full_honk_relation_value += lagrange_last * z_perm_shift * scaling_factor;
    };
};

// TODO(luke): The only difference between the Ultra relation and the Standard version is the enum
// used to refer into the edge polynomials. Seems desireable to not duplicate the code here but
// leaving this as is until Codys Flavor work is settled.
template <typename FF> class UltraGrandProductInitializationRelation {
  public:
    constexpr bool scale_by_random_polynomial() { return true; }

    // 1 + polynomial degree of this relation
    static constexpr size_t RELATION_LENGTH = 3;

    /**
     * @brief Add contribution of the permutation relation for a given edge
     *
     * @details There are 2 relations associated with enforcing the wire copy relations
     * This file handles the relation Z_perm_shift(n_last) = 0 via the relation:
     *
     *                      C(X) = L_LAST(X) * Z_perm_shift(X)
     *
     * @param evals transformed to `evals + C(extended_edges(X)...)*scaling_factor`
     * @param extended_edges an std::array containing the fully extended Univariate edges.
     * @param parameters contains beta, gamma, and public_input_delta, ....
     * @param scaling_factor optional term to scale the evaluation before adding to evals.
     */
    void add_edge_contribution(Univariate<FF, RELATION_LENGTH>& evals,
                               const auto& extended_edges,
                               const RelationParameters<FF>&,
                               const FF& scaling_factor) const
    {
        auto z_perm_shift = UnivariateView<FF, RELATION_LENGTH>(extended_edges.z_perm_shift);
        auto lagrange_last = UnivariateView<FF, RELATION_LENGTH>(extended_edges.lagrange_last);

        evals += (lagrange_last * z_perm_shift) * scaling_factor;
    };

    void add_full_relation_value_contribution(FF& full_honk_relation_value,
                                              auto& purported_evaluations,
                                              const RelationParameters<FF>&,
                                              const FF& scaling_factor) const
    {
        auto z_perm_shift = purported_evaluations.z_perm_shift;
        auto lagrange_last = purported_evaluations.lagrange_last;

        full_honk_relation_value += lagrange_last * z_perm_shift * scaling_factor;
    };
};
} // namespace proof_system::honk::sumcheck
