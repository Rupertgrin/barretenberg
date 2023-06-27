#pragma once
#include <array>
#include <tuple>

#include "../polynomials/univariate.hpp"
#include "relation.hpp"

namespace proof_system::honk::sumcheck {

template <typename FF> class ArithmeticRelation {
  public:
    constexpr bool scale_by_random_polynomial() { return true; }

    // 1 + polynomial degree of this relation
    static constexpr size_t RELATION_LENGTH = 4;

    /**
     * @brief Expression for the StandardArithmetic gate.
     * @details The relation is defined as C(extended_edges(X)...) =
     *    (q_m * w_r * w_l) + (q_l * w_l) + (q_r * w_r) + (q_o * w_o) + q_c
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
        // OPTIMIZATION?: Karatsuba in general, at least for some degrees?
        //       See https://hackmd.io/xGLuj6biSsCjzQnYN-pEiA?both

        auto w_l = UnivariateView<FF, RELATION_LENGTH>(extended_edges.w_l);
        auto w_r = UnivariateView<FF, RELATION_LENGTH>(extended_edges.w_r);
        auto w_o = UnivariateView<FF, RELATION_LENGTH>(extended_edges.w_o);
        auto q_m = UnivariateView<FF, RELATION_LENGTH>(extended_edges.q_m);
        auto q_l = UnivariateView<FF, RELATION_LENGTH>(extended_edges.q_l);
        auto q_r = UnivariateView<FF, RELATION_LENGTH>(extended_edges.q_r);
        auto q_o = UnivariateView<FF, RELATION_LENGTH>(extended_edges.q_o);
        auto q_c = UnivariateView<FF, RELATION_LENGTH>(extended_edges.q_c);

        auto tmp = w_l * (q_m * w_r + q_l);
        tmp += q_r * w_r;
        tmp += q_o * w_o;
        tmp += q_c;
        tmp *= scaling_factor;
        evals += tmp;
    };

    void add_full_relation_value_contribution(FF& full_honk_relation_value,
                                              const auto& purported_evaluations,
                                              const RelationParameters<FF>&,
                                              const FF& scaling_factor) const
    {
        auto w_l = purported_evaluations.w_l;
        auto w_r = purported_evaluations.w_r;
        auto w_o = purported_evaluations.w_o;
        auto q_m = purported_evaluations.q_m;
        auto q_l = purported_evaluations.q_l;
        auto q_r = purported_evaluations.q_r;
        auto q_o = purported_evaluations.q_o;
        auto q_c = purported_evaluations.q_c;

        full_honk_relation_value += w_l * (q_m * w_r + q_l) * scaling_factor;
        full_honk_relation_value += q_r * w_r * scaling_factor;
        full_honk_relation_value += q_o * w_o * scaling_factor;
        full_honk_relation_value += q_c * scaling_factor;
    };
};
} // namespace proof_system::honk::sumcheck
