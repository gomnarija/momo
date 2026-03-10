#include <gtest/gtest.h>
#include "momo.h"
#include "reader.h"

using namespace momo;

// Helper: parse + eval a string, return result
static moValPtr eval_str(const std::string& code, moEnv& env) {
	std::string input = code;
	moValPtr ast = readString(input);
	return eval(ast, env);
}

// --- Self-evaluating atoms ---

// Numbers evaluate to themselves
TEST(Eval, NumberSelfEval) {
	moEnv env;
	moValPtr result = eval_str("42", env);
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 42.0);
}

// Strings evaluate to themselves
TEST(Eval, StringSelfEval) {
	moEnv env;
	moValPtr result = eval_str("\"hello\"", env);
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_STRING);
	EXPECT_EQ(result->print(), "hello");
}

// Unbound symbol evaluates to NIL
TEST(Eval, UnboundSymbolIsNil) {
	moEnv env;
	moValPtr result = eval_str("x", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NIL);
}

// Bound symbol evaluates to its value
TEST(Eval, BoundSymbolResolvesValue) {
	moEnv env;
	env.insertVal("x", std::make_shared<moNumber>(99.0));
	moValPtr result = eval_str("x", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 99.0);
}

// --- List evaluation ---

// [+ 1 2] => 3
TEST(Eval, SumTwoNumbers) {
	moEnv env;
	moValPtr result = eval_str("[+ 1 2]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 3.0);
}

// [- 10 3] => 7
TEST(Eval, DifferenceTwoNumbers) {
	moEnv env;
	moValPtr result = eval_str("[- 10 3]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 7.0);
}

// Nested: [+ [- 10 5] 3] => 8
TEST(Eval, NestedExpression) {
	moEnv env;
	moValPtr result = eval_str("[+ [- 10 5] 3]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 8.0);
}

// Multiple arguments: [+ 1 2 3 4] => 10
TEST(Eval, SumMultipleNumbers) {
	moEnv env;
	moValPtr result = eval_str("[+ 1 2 3 4]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 10.0);
}

// String concatenation: [+ "hello" " world"] => "hello world"
TEST(Eval, SumStringsConcat) {
	moEnv env;
	moValPtr result = eval_str("[+ \"hello\" \" world\"]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_STRING);
	EXPECT_EQ(result->print(), "hello world");
}

// Empty list evaluates to NIL
TEST(Eval, EmptyListIsNil) {
	moEnv env;
	moValPtr result = eval_str("[]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NIL);
}

// Symbol in operator position resolves from env
TEST(Eval, SymbolResolvesInArgs) {
	moEnv env;
	env.insertVal("a", std::make_shared<moNumber>(10.0));
	env.insertVal("b", std::make_shared<moNumber>(20.0));
	moValPtr result = eval_str("[+ a b]", env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 30.0);
}

// TODO: Phase 3 — označi
// TEST(Eval, Oznaci) {
//     // [označi x 5] should bind x to 5 in env
// }

// TODO: Phase 3 — ako (if)
// TEST(Eval, AkoTrue) { ... }
// TEST(Eval, AkoFalse) { ... }

// TODO: Phase 3 — funkcija
// TEST(Eval, FunkcijaDefAndCall) { ... }

// TODO: Phase 3 — arithmetic operators *, /
// TEST(Eval, Multiply) { ... }
// TEST(Eval, Divide) { ... }

// TODO: Phase 3 — comparison operators =, >, <
// TEST(Eval, Equals) { ... }
// TEST(Eval, GreaterThan) { ... }
// TEST(Eval, LessThan) { ... }
