#pragma once
#include "barretenberg/smt_verification/solver/solver.hpp"

namespace smt_terms {
using namespace smt_solver;

class FFTerm {
  public:
    Solver* solver;
    cvc5::Term term;
    bool isconst;

    FFTerm()
        : solver(nullptr)
        , term(cvc5::Term())
        , isconst(false){};
    explicit FFTerm(const std::string& t, Solver* slv, bool isconst = false, uint32_t base = 16);
    FFTerm(cvc5::Term& term, Solver* s)
        : solver(s)
        , term(term){};
    FFTerm(const FFTerm& other) = default;
    FFTerm(FFTerm&& other) = default;

    FFTerm& operator=(const FFTerm& right) = default;
    FFTerm& operator=(FFTerm&& right) = default;

    FFTerm operator+(const FFTerm& other) const;
    void operator+=(const FFTerm& other);
    FFTerm operator-(const FFTerm& other) const;
    void operator-=(const FFTerm& other);
    FFTerm operator*(const FFTerm& other) const;
    void operator*=(const FFTerm& other);
    FFTerm operator/(const FFTerm& other) const;
    void operator/=(const FFTerm& other);

    void operator==(const FFTerm& other) const;
    void operator!=(const FFTerm& other) const;

    // TODO(alex): Maybe do the same thing with +, - but I don't see a point
    // and also properlythink on how to implement this sh
    // void operator==(const std::pair<std::string, uint32_t>& other) const;
    // void operator!=(const std::pair<std::string, uint32_t>& other) const;

    operator std::string() const { return isconst ? term.getFiniteFieldValue() : term.toString(); };
    operator cvc5::Term() const { return term; };

    ~FFTerm() = default;
    friend std::ostream& operator<<(std::ostream& out, const FFTerm& k) { return out << k.term; }

    friend FFTerm batch_add(const std::vector<FFTerm>& children)
    {
        Solver* slv = children[0].solver;
        std::vector<cvc5::Term> terms(children.begin(), children.end());
        cvc5::Term res = slv->s.mkTerm(cvc5::Kind::FINITE_FIELD_ADD, terms);
        return { res, slv };
    }

    friend FFTerm batch_mul(const std::vector<FFTerm>& children)
    {
        Solver* slv = children[0].solver;
        std::vector<cvc5::Term> terms(children.begin(), children.end());
        cvc5::Term res = slv->s.mkTerm(cvc5::Kind::FINITE_FIELD_MULT, terms);
        return { res, slv };
    }
};

FFTerm Var(const std::string& name, Solver* slv);
FFTerm Const(const std::string& val, Solver* slv, uint32_t base = 16);

} // namespace smt_terms