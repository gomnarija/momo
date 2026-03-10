#include <gtest/gtest.h>
#include "momo.h"
#include "reader.h"

using namespace momo;

// Helper: parse + eval a string
static moValPtr eval_str(const std::string& code, moEnv& env) {
	std::string input = code;
	moValPtr ast = readString(input);
	return eval(ast, env);
}

// --- Product (*) ---

// [* 3 4] => 12
TEST(OpProduct, TwoNumbers) {
	moEnv env;
	auto r = eval_str("[* 3 4]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 12.0);
}

// [* 2 3 5] => 30
TEST(OpProduct, MultipleNumbers) {
	moEnv env;
	auto r = eval_str("[* 2 3 5]", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 30.0);
}

// [* 5 "3"] => 15 (string converts to number)
TEST(OpProduct, StringConvertsToNumber) {
	moEnv env;
	auto r = eval_str("[* 5 \"3\"]", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 15.0);
}

// [*] with no args => NIL
TEST(OpProduct, NoArgs) {
	moEnv env;
	auto r = eval_str("[*]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
}

// --- Quotient (/) ---

// [/ 10 2] => 5
TEST(OpQuotient, TwoNumbers) {
	moEnv env;
	auto r = eval_str("[/ 10 2]", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 5.0);
}

// [/ 100 2 5] => 10 (100/2/5)
TEST(OpQuotient, MultipleNumbers) {
	moEnv env;
	auto r = eval_str("[/ 100 2 5]", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 10.0);
}

// [/ 10 0] => division by zero handling
TEST(OpQuotient, DivisionByZero) {
	moEnv env;
	Logger::instance().clear();
	auto r = eval_str("[/ 10 0]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
	EXPECT_TRUE(Logger::instance().hasErrors());
	Logger::instance().clear();
}

// [/ "hello" 2] => NIL (first arg must be number)
TEST(OpQuotient, StringFirstArg) {
	moEnv env;
	Logger::instance().clear();
	auto r = eval_str("[/ \"hello\" 2]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
	Logger::instance().clear();
}

// --- Truthiness (?) ---

// [? 1] => tačno (nonzero number)
TEST(OpTruth, NonzeroNumber) {
	moEnv env;
	auto r = eval_str("[? 1]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(r->isTrue());
}

// [? 0] => netačno (zero)
TEST(OpTruth, ZeroNumber) {
	moEnv env;
	auto r = eval_str("[? 0]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(r->isTrue());
}

// [? "hello"] => tačno (non-empty string)
TEST(OpTruth, NonEmptyString) {
	moEnv env;
	auto r = eval_str("[? \"hello\"]", env);
	EXPECT_TRUE(r->isTrue());
}

// [? ""] => netačno (empty string)
TEST(OpTruth, EmptyString) {
	moEnv env;
	auto r = eval_str("[? \"\"]", env);
	EXPECT_FALSE(r->isTrue());
}

// [? tačno] => tačno
TEST(OpTruth, TacnoIsTrue) {
	moEnv env;
	auto r = eval_str("[? tačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [? netačno] => netačno
TEST(OpTruth, NetacnoIsFalse) {
	moEnv env;
	auto r = eval_str("[? netačno]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Logical not (!) ---

// [! tačno] => netačno
TEST(OpNot, NotTrue) {
	moEnv env;
	auto r = eval_str("[! tačno]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(r->isTrue());
}

// [! netačno] => tačno
TEST(OpNot, NotFalse) {
	moEnv env;
	auto r = eval_str("[! netačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [! 0] => tačno (0 is falsy)
TEST(OpNot, NotZero) {
	moEnv env;
	auto r = eval_str("[! 0]", env);
	EXPECT_TRUE(r->isTrue());
}

// [! 42] => netačno (nonzero is truthy)
TEST(OpNot, NotNonzero) {
	moEnv env;
	auto r = eval_str("[! 42]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Equality (=) ---

// [= 5 5] => tačno
TEST(OpEquals, EqualNumbers) {
	moEnv env;
	auto r = eval_str("[= 5 5]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(r->isTrue());
}

// [= 5 6] => netačno
TEST(OpEquals, DifferentNumbers) {
	moEnv env;
	auto r = eval_str("[= 5 6]", env);
	EXPECT_FALSE(r->isTrue());
}

// [= "abc" "abc"] => tačno
TEST(OpEquals, EqualStrings) {
	moEnv env;
	auto r = eval_str("[= \"abc\" \"abc\"]", env);
	EXPECT_TRUE(r->isTrue());
}

// [= "abc" "xyz"] => netačno
TEST(OpEquals, DifferentStrings) {
	moEnv env;
	auto r = eval_str("[= \"abc\" \"xyz\"]", env);
	EXPECT_FALSE(r->isTrue());
}

// [= tačno tačno] => tačno
TEST(OpEquals, BoolsEqual) {
	moEnv env;
	auto r = eval_str("[= tačno tačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [= 1 "1"] => netačno (different types)
TEST(OpEquals, DifferentTypes) {
	moEnv env;
	auto r = eval_str("[= 1 \"1\"]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Strict equality (==) ---

// same number values are == (since we can't test pointer identity easily,
// this tests that two separately created equal numbers are NOT ==)
TEST(OpStrictEquals, DifferentObjectsSameValue) {
	moEnv env;
	auto r = eval_str("[== 5 5]", env);
	// Two different number objects — not the same pointer
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(r->isTrue());
}

// --- Greater than (>) ---

// [> 5 3] => tačno
TEST(OpGreater, FiveGreaterThanThree) {
	moEnv env;
	auto r = eval_str("[> 5 3]", env);
	EXPECT_TRUE(r->isTrue());
}

// [> 3 5] => netačno
TEST(OpGreater, ThreeNotGreaterThanFive) {
	moEnv env;
	auto r = eval_str("[> 3 5]", env);
	EXPECT_FALSE(r->isTrue());
}

// [> 5 5] => netačno (not strictly greater)
TEST(OpGreater, EqualNotGreater) {
	moEnv env;
	auto r = eval_str("[> 5 5]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Less than (<) ---

// [< 3 5] => tačno
TEST(OpLess, ThreeLessThanFive) {
	moEnv env;
	auto r = eval_str("[< 3 5]", env);
	EXPECT_TRUE(r->isTrue());
}

// [< 5 3] => netačno
TEST(OpLess, FiveNotLessThanThree) {
	moEnv env;
	auto r = eval_str("[< 5 3]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Greater or equal (>=) ---

// [>= 5 3] => tačno
TEST(OpGreaterEq, Greater) {
	moEnv env;
	auto r = eval_str("[>= 5 3]", env);
	EXPECT_TRUE(r->isTrue());
}

// [>= 5 5] => tačno
TEST(OpGreaterEq, Equal) {
	moEnv env;
	auto r = eval_str("[>= 5 5]", env);
	EXPECT_TRUE(r->isTrue());
}

// [>= 3 5] => netačno
TEST(OpGreaterEq, Less) {
	moEnv env;
	auto r = eval_str("[>= 3 5]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Less or equal (<=) ---

// [<= 3 5] => tačno
TEST(OpLessEq, Less) {
	moEnv env;
	auto r = eval_str("[<= 3 5]", env);
	EXPECT_TRUE(r->isTrue());
}

// [<= 5 5] => tačno
TEST(OpLessEq, Equal) {
	moEnv env;
	auto r = eval_str("[<= 5 5]", env);
	EXPECT_TRUE(r->isTrue());
}

// [<= 5 3] => netačno
TEST(OpLessEq, Greater) {
	moEnv env;
	auto r = eval_str("[<= 5 3]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Logical and (&) — short-circuit ---

// [& tačno tačno] => tačno
TEST(OpAnd, BothTrue) {
	moEnv env;
	auto r = eval_str("[& tačno tačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [& tačno netačno] => netačno
TEST(OpAnd, SecondFalse) {
	moEnv env;
	auto r = eval_str("[& tačno netačno]", env);
	EXPECT_FALSE(r->isTrue());
}

// [& netačno tačno] => netačno (short-circuits, doesn't eval second)
TEST(OpAnd, FirstFalse) {
	moEnv env;
	auto r = eval_str("[& netačno tačno]", env);
	EXPECT_FALSE(r->isTrue());
}

// [& 1 2] => truthy (both truthy, returns last)
TEST(OpAnd, BothTruthyNumbers) {
	moEnv env;
	auto r = eval_str("[& 1 2]", env);
	EXPECT_TRUE(r->isTrue());
}

// [& 0 1] => falsy (first is falsy)
TEST(OpAnd, FirstFalsyNumber) {
	moEnv env;
	auto r = eval_str("[& 0 1]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Logical or (|) — short-circuit ---

// [| netačno tačno] => tačno
TEST(OpOr, SecondTrue) {
	moEnv env;
	auto r = eval_str("[| netačno tačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [| tačno netačno] => tačno (short-circuits, returns first truthy)
TEST(OpOr, FirstTrue) {
	moEnv env;
	auto r = eval_str("[| tačno netačno]", env);
	EXPECT_TRUE(r->isTrue());
}

// [| netačno netačno] => netačno
TEST(OpOr, BothFalse) {
	moEnv env;
	auto r = eval_str("[| netačno netačno]", env);
	EXPECT_FALSE(r->isTrue());
}

// [| 0 5] => truthy (first falsy, returns second)
TEST(OpOr, FirstFalsySecondTruthy) {
	moEnv env;
	auto r = eval_str("[| 0 5]", env);
	EXPECT_TRUE(r->isTrue());
}

// [| 0 0] => falsy
TEST(OpOr, BothFalsyNumbers) {
	moEnv env;
	auto r = eval_str("[| 0 0]", env);
	EXPECT_FALSE(r->isTrue());
}

// --- Nested operator expressions ---

// [> [+ 2 3] [* 1 4]] => [> 5 4] => tačno
TEST(OpCombo, NestedArithmetic) {
	moEnv env;
	auto r = eval_str("[> [+ 2 3] [* 1 4]]", env);
	EXPECT_TRUE(r->isTrue());
}

// [! [= 3 4]] => tačno
TEST(OpCombo, NotEquals) {
	moEnv env;
	auto r = eval_str("[! [= 3 4]]", env);
	EXPECT_TRUE(r->isTrue());
}

// [& [> 5 3] [< 1 2]] => tačno
TEST(OpCombo, AndWithComparisons) {
	moEnv env;
	auto r = eval_str("[& [> 5 3] [< 1 2]]", env);
	EXPECT_TRUE(r->isTrue());
}
