#include <gtest/gtest.h>
#include <momo.h>
#include <reader.h>

using namespace momo;

// helper: parse and eval a string
static moValPtr run(const std::string& code, moEnv& env){
    std::string s = code;
    moValPtr ast = readString(s);
    return eval(ast, env);
}


// --- basic ako ---

// [ako tačno 1] → 1
TEST(Ako, TrueConditionReturnsOnda){
    moEnv env;
    moValPtr r = run("[ako tačno 1]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 1.0);
}

// [ako netačno 1] → NIL (no else branch)
TEST(Ako, FalseConditionNoElseReturnsNil){
    moEnv env;
    moValPtr r = run("[ako netačno 1]", env);
    EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
}

// [ako tačno 1 2] → 1
TEST(Ako, TrueConditionWithElseReturnsThen){
    moEnv env;
    moValPtr r = run("[ako tačno 1 2]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 1.0);
}

// [ako netačno 1 2] → 2
TEST(Ako, FalseConditionWithElseReturnsElse){
    moEnv env;
    moValPtr r = run("[ako netačno 1 2]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 2.0);
}


// --- condition is evaluated ---

// [ako [> 5 3] "da" "ne"] → "da"
TEST(Ako, EvaluatesConditionExpression){
    moEnv env;
    moValPtr r = run("[ako [> 5 3] \"da\" \"ne\"]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_STRING);
    EXPECT_EQ(r->print(), "da");
}

// [ako [< 5 3] "da" "ne"] → "ne"
TEST(Ako, FalseExpressionCondition){
    moEnv env;
    moValPtr r = run("[ako [< 5 3] \"da\" \"ne\"]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_STRING);
    EXPECT_EQ(r->print(), "ne");
}


// --- branches are expressions ---

// [ako tačno [+ 1 2] [+ 3 4]] → 3
TEST(Ako, ThenBranchIsExpression){
    moEnv env;
    moValPtr r = run("[ako tačno [+ 1 2] [+ 3 4]]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 3.0);
}

// [ako netačno [+ 1 2] [+ 3 4]] → 7
TEST(Ako, ElseBranchIsExpression){
    moEnv env;
    moValPtr r = run("[ako netačno [+ 1 2] [+ 3 4]]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 7.0);
}


// --- truthiness of non-boolean values ---

// [ako 1 "da" "ne"] → "da" (numbers are truthy)
TEST(Ako, NumberIsTruthy){
    moEnv env;
    moValPtr r = run("[ako 1 \"da\" \"ne\"]", env);
    EXPECT_EQ(r->print(), "da");
}

// [ako 0 "da" "ne"] → "ne" (0 is falsy)
TEST(Ako, ZeroIsFalsy){
    moEnv env;
    moValPtr r = run("[ako 0 \"da\" \"ne\"]", env);
    EXPECT_EQ(r->print(), "ne");
}

// [ako "" "da" "ne"] → "ne" (empty string is falsy)
TEST(Ako, EmptyStringIsFalsy){
    moEnv env;
    moValPtr r = run("[ako \"\" \"da\" \"ne\"]", env);
    EXPECT_EQ(r->print(), "ne");
}


// --- nested ako ---

// [ako tačno [ako netačno 1 2] 3] → 2
TEST(Ako, NestedAko){
    moEnv env;
    moValPtr r = run("[ako tačno [ako netačno 1 2] 3]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 2.0);
}


// --- ako with variables ---

// :x 10  [ako [> x 5] "veliko" "malo"] → "veliko"
TEST(Ako, WithVariables){
    moEnv env;
    run(":x 10", env);
    moValPtr r = run("[ako [> x 5] \"veliko\" \"malo\"]", env);
    EXPECT_EQ(r->print(), "veliko");
}


// --- only chosen branch is evaluated (side effects) ---

// [ako tačno [:a 1] [:a 2]] — a should be 1, not 2
TEST(Ako, OnlyTrueBranchEvaluated){
    moEnv env;
    run("[ako tačno [:a 1] [:a 2]]", env);
    moValPtr a = run("a", env);
    ASSERT_EQ(a->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(a)->getValue(), 1.0);
}

// [ako netačno [:b 1] [:b 2]] — b should be 2
TEST(Ako, OnlyFalseBranchEvaluated){
    moEnv env;
    run("[ako netačno [:b 1] [:b 2]]", env);
    moValPtr b = run("b", env);
    ASSERT_EQ(b->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(b)->getValue(), 2.0);
}
