#include "representation/Contract.hh"
#include "representation/Variable.hh"
#include "representation/Boolean.hh"
#include "representation/Proposition.hh"
#include "representation/BooleanConstant.hh"
#include "representation/UnaryBooleanFormula.hh"
#include "representation/BinaryBooleanFormula.hh"
#include "utilities/Factory.hh"
#include <gtest/gtest.h>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>

using namespace chase;

// Semantic tests of Contract::quotient and Contract::separation on
// propositional contracts over the
// Boolean variables p, q, r. A propositional formula is represented by its
// truth table: bit i of the mask is the value of the formula under the
// assignment p = i&1, q = i&2, r = i&4.

namespace {

using Mask = unsigned;
const Mask ALL = 0xFFu;

bool eval(LogicFormula *f, const std::map<std::string, bool> &env) {
    if (auto c = dynamic_cast<BooleanConstant *>(f))
        return c->getValue();
    if (auto p = dynamic_cast<Proposition *>(f))
        return env.at(p->getName()->getString());
    if (auto u = dynamic_cast<UnaryBooleanFormula *>(f)) {
        if (u->getOp() != op_not) throw std::runtime_error("unsupported op");
        return !eval(u->getOp1(), env);
    }
    if (auto b = dynamic_cast<BinaryBooleanFormula *>(f)) {
        bool x = eval(b->getOp1(), env);
        bool y = eval(b->getOp2(), env);
        switch (b->getOp()) {
            case op_and: return x && y;
            case op_or: return x || y;
            case op_implies: return !x || y;
            case op_iff: return x == y;
            default: throw std::runtime_error("unsupported op");
        }
    }
    throw std::runtime_error("unsupported formula");
}

Mask mask(Specification *s) {
    if (s == nullptr) return ALL;  // missing specification means true
    auto f = dynamic_cast<LogicFormula *>(s);
    if (f == nullptr) throw std::runtime_error("not a logic formula");
    Mask m = 0;
    for (unsigned i = 0; i < 8; ++i) {
        std::map<std::string, bool> env{
            {"p", (i & 1u) != 0}, {"q", (i & 2u) != 0}, {"r", (i & 4u) != 0}};
        if (eval(f, env)) m |= (1u << i);
    }
    return m;
}

struct AG { Mask a, g; };

AG masks(Contract *c) {
    auto a = c->assumptions.find(logic);
    auto g = c->guarantees.find(logic);
    return {mask(a == c->assumptions.end() ? nullptr : a->second),
            mask(g == c->guarantees.end() ? nullptr : g->second)};
}

AG sat(AG c) { return {c.a, (c.g | ~c.a) & ALL}; }

// C <= D (saturated): A_D -> A_C and G_C -> G_D.
bool refines(AG c, AG d) {
    return (d.a & ~c.a & ALL) == 0 && (c.g & ~d.g & ALL) == 0;
}

// Merging (A1 & A2, G1 & G2), saturated.
AG merge(AG c1, AG c2) { return sat({c1.a & c2.a, c1.g & c2.g}); }

// Composition ((A1 & A2) | !(G1 & G2), G1 & G2).
AG compose(AG c1, AG c2) {
    Mask g = c1.g & c2.g;
    return {((c1.a & c2.a) | ~g) & ALL, g};
}

using Builder = std::function<LogicFormula *(Variable *, Variable *, Variable *)>;

// Build a contract over fresh Boolean variables p, q, r.
Contract *makeContract(const std::string &name, Builder a, Builder g) {
    auto c = new Contract(name);
    auto p = new Variable(new Boolean(), new Name("p"), input);
    auto q = new Variable(new Boolean(), new Name("q"), input);
    auto r = new Variable(new Boolean(), new Name("r"), output);
    c->declarations.push_back(p);
    c->declarations.push_back(q);
    c->declarations.push_back(r);
    if (a) c->addAssumptions(logic, a(p, q, r));
    if (g) c->addGuarantees(logic, g(p, q, r));
    return c;
}

names_projection_map sameNames() {
    return {{"p", "p"}, {"q", "q"}, {"r", "r"}};
}

// Checks, for saturated c1 and c2:
//  - the result equals ((A1 & G2) | !(G1 & A2), G1 & A2) and is saturated;
//  - C1 <= (C1 % C2) . C2;
//  - Galois property: for every saturated E,  C1 % C2 <= E  iff  C1 <= E . C2.
// A contract without guarantees (G = TRUE) is already saturated; it is not
// passed to Contract::saturate, which does not handle a missing guarantee.
void saturateIfNeeded(Contract *c) {
    if (c->guarantees.find(logic) != c->guarantees.end())
        Contract::saturate(c);
}

void checkSeparation(Contract *c1, Contract *c2) {
    saturateIfNeeded(c1);
    saturateIfNeeded(c2);
    auto m = sameNames();
    Contract *s = Contract::separation(c1, c2, m);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->getName()->getString(), "separation");

    AG k1 = masks(c1), k2 = masks(c2), ks = masks(s);
    Mask expA = ((k1.a & k2.g) | ~(k1.g & k2.a)) & ALL;
    Mask expG = k1.g & k2.a;
    EXPECT_EQ(ks.a, expA);
    EXPECT_EQ(ks.g, expG);
    EXPECT_EQ((~ks.a & ~ks.g) & ALL, 0u) << "result not saturated";

    // Also through the library's own merging operation.
    Contract *ms = Contract::merging(s, c2, m);
    EXPECT_TRUE(refines(k1, sat(masks(ms))));
    EXPECT_TRUE(refines(k1, merge(ks, k2)));

    for (Mask a = 0; a <= ALL; ++a)
        for (Mask g = 0; g <= ALL; ++g) {
            AG e = sat({a, g});
            ASSERT_EQ(refines(ks, e), refines(k1, merge(e, k2)))
                << "Galois property violated for E = (" << a << ", " << g
                << ")";
        }
}

// Checks, for saturated c1 and c2:
//  - the result equals (A1 & G2, (G1 & A2) | !(A1 & G2));
//  - with synthesizable set, the guarantees are G1 & A2;
//  - C2 || (C1 / C2) <= C1;
//  - Galois property: for every saturated E,  E <= C1 / C2  iff  C2 || E <= C1.
void checkQuotient(Contract *c1, Contract *c2) {
    saturateIfNeeded(c1);
    saturateIfNeeded(c2);
    auto m = sameNames();
    Contract *q = Contract::quotient(c1, c2, m);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getName()->getString(), "quotient");

    AG k1 = masks(c1), k2 = masks(c2), kq = masks(q);
    Mask expA = k1.a & k2.g;
    Mask expG = ((k1.g & k2.a) | ~expA) & ALL;
    EXPECT_EQ(kq.a, expA);
    EXPECT_EQ(kq.g, expG);

    auto ms = sameNames();
    AG ks = masks(Contract::quotient(c1, c2, ms, "q", true));
    EXPECT_EQ(ks.a, expA);
    EXPECT_EQ(ks.g, k1.g & k2.a);

    Contract *cq = Contract::composition(c2, q, m);
    EXPECT_TRUE(refines(sat(masks(cq)), k1));
    EXPECT_TRUE(refines(compose(k2, kq), k1));

    for (Mask a = 0; a <= ALL; ++a)
        for (Mask g = 0; g <= ALL; ++g) {
            AG e = sat({a, g});
            ASSERT_EQ(refines(e, kq), refines(compose(k2, e), k1))
                << "Galois property violated for E = (" << a << ", " << g
                << ")";
        }
}

}  // namespace

TEST(SeparationTest, PropositionalExample) {
    auto c1 = makeContract("C1",
        [](Variable *p, Variable *, Variable *) { return Prop(p); },
        [](Variable *, Variable *q, Variable *) { return Prop(q); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *q, Variable *) { return Prop(q); },
        [](Variable *, Variable *, Variable *r) { return Prop(r); });
    checkSeparation(c1, c2);
}

TEST(SeparationTest, MixedConnectives) {
    auto c1 = makeContract("C1",
        [](Variable *p, Variable *q, Variable *) {
            return Or(Prop(p), Prop(q)); },
        [](Variable *p, Variable *, Variable *r) {
            return Implies(Prop(p), Prop(r)); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *q, Variable *r) {
            return And(Prop(q), Not(Prop(r))); },
        [](Variable *p, Variable *q, Variable *) {
            return Iff(Prop(p), Prop(q)); });
    checkSeparation(c1, c2);
}

TEST(SeparationTest, MissingSpecificationsAreTrue) {
    // C1 has no assumptions, C2 has no guarantees.
    auto c1 = makeContract("C1", nullptr,
        [](Variable *p, Variable *q, Variable *) {
            return And(Prop(p), Prop(q)); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *, Variable *r) { return Prop(r); }, nullptr);
    checkSeparation(c1, c2);

    // Both contracts empty: separation is (TRUE, TRUE).
    auto e1 = makeContract("E1", nullptr, nullptr);
    auto e2 = makeContract("E2", nullptr, nullptr);
    auto m = sameNames();
    Contract *s = Contract::separation(e1, e2, m, "sep");
    EXPECT_EQ(s->getName()->getString(), "sep");
    AG ks = masks(s);
    EXPECT_EQ(ks.a, ALL);
    EXPECT_EQ(ks.g, ALL);
}

TEST(SeparationTest, SharedDeclarations) {
    auto c1 = makeContract("C1",
        [](Variable *p, Variable *, Variable *) { return Prop(p); },
        [](Variable *, Variable *q, Variable *) { return Prop(q); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *q, Variable *) { return Prop(q); },
        [](Variable *, Variable *, Variable *r) { return Prop(r); });
    auto m = sameNames();
    Contract *s = Contract::separation(c1, c2, m);
    // Variables with the same name are merged, not duplicated.
    EXPECT_EQ(s->declarations.size(), 3u);
}

TEST(QuotientTest, PropositionalExample) {
    // A1 = G1 = A2 = G2 = TRUE except A2 = q: the case where the formula
    // without A2 in the guarantees fails.
    auto c1 = makeContract("C1",
        [](Variable *p, Variable *, Variable *) { return Prop(p); },
        [](Variable *, Variable *, Variable *r) { return Prop(r); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *q, Variable *) { return Prop(q); },
        [](Variable *p, Variable *, Variable *) { return Prop(p); });
    checkQuotient(c1, c2);
}

TEST(QuotientTest, MixedConnectives) {
    auto c1 = makeContract("C1",
        [](Variable *p, Variable *q, Variable *) {
            return Or(Prop(p), Prop(q)); },
        [](Variable *p, Variable *, Variable *r) {
            return Implies(Prop(p), Prop(r)); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *q, Variable *r) {
            return And(Prop(q), Not(Prop(r))); },
        [](Variable *p, Variable *q, Variable *) {
            return Iff(Prop(p), Prop(q)); });
    checkQuotient(c1, c2);
}

TEST(QuotientTest, MissingSpecificationsAreTrue) {
    auto c1 = makeContract("C1", nullptr,
        [](Variable *p, Variable *q, Variable *) {
            return And(Prop(p), Prop(q)); });
    auto c2 = makeContract("C2",
        [](Variable *, Variable *, Variable *r) { return Prop(r); }, nullptr);
    checkQuotient(c1, c2);
}
