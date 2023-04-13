#include "acir_format.hpp"
#include "recursion_constraint.hpp"
#include "barretenberg/plonk/proof_system/types/proof.hpp"
#include "barretenberg/plonk/proof_system/verification_key/verification_key.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace proof_system::plonk::stdlib::types;

Composer create_inner_circuit()
{
    /**
     * constraints produced by Noir program:
     * fn main(x : u32, y : pub u32) {
     * let z = x ^ y;
     *
     * constrain z != 10;
     * }
     **/
    acir_format::RangeConstraint range_a{
        .witness = 1,
        .num_bits = 32,
    };
    acir_format::RangeConstraint range_b{
        .witness = 2,
        .num_bits = 32,
    };

    acir_format::LogicConstraint logic_constraint{
        .a = 1,
        .b = 2,
        .result = 3,
        .num_bits = 32,
        .is_xor_gate = 1,
    };
    poly_triple expr_a{
        .a = 3,
        .b = 4,
        .c = 0,
        .q_m = 0,
        .q_l = 1,
        .q_r = -1,
        .q_o = 0,
        .q_c = -10,
    };
    poly_triple expr_b{
        .a = 4,
        .b = 5,
        .c = 6,
        .q_m = 1,
        .q_l = 0,
        .q_r = 0,
        .q_o = -1,
        .q_c = 0,
    };
    poly_triple expr_c{
        .a = 4,
        .b = 6,
        .c = 4,
        .q_m = 1,
        .q_l = 0,
        .q_r = 0,
        .q_o = -1,
        .q_c = 0,

    };
    poly_triple expr_d{
        .a = 6,
        .b = 0,
        .c = 0,
        .q_m = 0,
        .q_l = -1,
        .q_r = 0,
        .q_o = 0,
        .q_c = 1,
    };
    // EXPR [ (1, _4, _5) (-1, _6) 0 ]
    // EXPR [ (1, _4, _6) (-1, _4) 0 ]
    // EXPR [ (-1, _6) 1 ]

    acir_format::acir_format constraint_system{
        .varnum = 7,
        .public_inputs = { 2 },
        .fixed_base_scalar_mul_constraints = {},
        .logic_constraints = { logic_constraint },
        .range_constraints = { range_a, range_b },
        .schnorr_constraints = {},
        .ecdsa_constraints = {},
        .sha256_constraints = {},
        .blake2s_constraints = {},
        .hash_to_field_constraints = {},
        .pedersen_constraints = {},
        .merkle_membership_constraints = {},
        .constraints = { expr_a, expr_b, expr_c, expr_d },
    };

    uint256_t inverse_of_five = fr(5).invert();
    auto composer = acir_format::create_circuit_with_witness(constraint_system,
                                                             {
                                                                 5,
                                                                 10,
                                                                 15,
                                                                 5,
                                                                 inverse_of_five,
                                                                 1,
                                                             });

    return composer;
}

TEST(RecursionConstraint, TestRecursionConstraint)
{
    auto inner_composer = create_inner_circuit();

    auto inner_prover = inner_composer.create_prover();
    auto inner_proof = inner_prover.construct_proof();
    auto inner_verifier = inner_composer.create_verifier();

    std::vector<uint8_t> keybuf;
    write(keybuf, *(inner_verifier.key));

    std::array<uint32_t, acir_format::RecursionConstraint::AGGREGATION_OBJECT_SIZE> output_vars;
    for (size_t i = 0; i < 16; ++i) {
        // variable idx 1 = public input
        // variable idx 2-18 = output_vars
        output_vars[i] = (static_cast<uint32_t>(i + 2));
    }
    verification_key_data keydata;
    uint8_t const* vk_buf = &keybuf[0];
    read(vk_buf, keydata);

    acir_format::RecursionConstraint recursion_constraint{
        .verification_key_data = keydata,
        .proof = inner_proof,
        .is_aggregation_object_nonzero = false,
        .public_input = 1,
        .input_aggregation_object = {},
        .output_aggregation_object = output_vars,
    };

    std::vector<fr> witness;
    for (size_t i = 0; i < 18; ++i) {
        witness.emplace_back(0);
    }

    acir_format::acir_format constraint_system{
        .varnum = 18,
        .public_inputs = { 1 },
        .fixed_base_scalar_mul_constraints = {},
        .logic_constraints = {},
        .range_constraints = {},
        .schnorr_constraints = {},
        .ecdsa_constraints = {},
        .sha256_constraints = {},
        .blake2s_constraints = {},
        .hash_to_field_constraints = {},
        .pedersen_constraints = {},
        .merkle_membership_constraints = {},
        .recursion_constraints = { recursion_constraint },
        .constraints = {},
    };

    auto composer = acir_format::create_circuit_with_witness(constraint_system, witness);
    auto prover = composer.create_prover();

    auto proof = prover.construct_proof();
    auto verifier = composer.create_verifier();
    EXPECT_EQ(verifier.verify_proof(proof), true);

    EXPECT_EQ(composer.get_variable(1), 10);
}