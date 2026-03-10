#include <gtest/gtest.h>
#include "momo.h"
#include "reader.h"

using namespace momo;

static moValPtr eval_str(const std::string& code, moEnv& env) {
	std::string input = code;
	moValPtr ast = readString(input);
	return eval(ast, env);
}

// Basic binding: [označi x 5] then x => 5
TEST(Oznaci, BasicBinding) {
	moEnv env;
	eval_str("[označi x 5]", env);
	auto r = eval_str("x", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 5.0);
}

// Bind a string
TEST(Oznaci, BindString) {
	moEnv env;
	eval_str("[označi ime \"momo\"]", env);
	auto r = eval_str("ime", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_STRING);
	EXPECT_EQ(r->print(), "momo");
}

// Bind a boolean
TEST(Oznaci, BindBool) {
	moEnv env;
	eval_str("[označi flag tačno]", env);
	auto r = eval_str("flag", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(r->isTrue());
}

// Overwrite existing binding in same scope
TEST(Oznaci, OverwriteExisting) {
	moEnv env;
	eval_str("[označi x 1]", env);
	eval_str("[označi x 99]", env);
	auto r = eval_str("x", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 99.0);
}

// Value is evaluated before binding: [označi x [+ 2 3]] => x = 5
TEST(Oznaci, ValueIsEvaluated) {
	moEnv env;
	eval_str("[označi x [+ 2 3]]", env);
	auto r = eval_str("x", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 5.0);
}

// Bind result of comparison: [označi r [> 5 3]] => r = tačno
TEST(Oznaci, BindComparisonResult) {
	moEnv env;
	eval_str("[označi r [> 5 3]]", env);
	auto r = eval_str("r", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(r->isTrue());
}

// Use bound variable in expression: [označi x 10] then [+ x 5] => 15
TEST(Oznaci, UseInExpression) {
	moEnv env;
	eval_str("[označi x 10]", env);
	auto r = eval_str("[+ x 5]", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 15.0);
}

// Chain bindings: [označi a 3] [označi b [* a 4]] => b = 12
TEST(Oznaci, ChainBindings) {
	moEnv env;
	eval_str("[označi a 3]", env);
	eval_str("[označi b [* a 4]]", env);
	auto r = eval_str("b", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 12.0);
}

// Child scope shadows parent
TEST(Oznaci, ChildShadowsParent) {
	moEnv parent;
	eval_str("[označi x 1]", parent);
	moEnv child(&parent);
	eval_str("[označi x 99]", child);
	// Child sees 99
	auto cr = eval_str("x", child);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(cr)->getValue(), 99.0);
	// Parent still sees 1
	auto pr = eval_str("x", parent);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(pr)->getValue(), 1.0);
}

// Child scope can see parent binding (if not shadowed)
TEST(Oznaci, ChildSeesParent) {
	moEnv parent;
	eval_str("[označi x 42]", parent);
	moEnv child(&parent);
	auto r = eval_str("x", child);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 42.0);
}

// Returns the bound value
TEST(Oznaci, ReturnsValue) {
	moEnv env;
	auto r = eval_str("[označi x 7]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 7.0);
}

// Missing value: [označi x] => error
TEST(Oznaci, MissingValue) {
	moEnv env;
	Logger::instance().clear();
	auto r = eval_str("[označi x]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
	EXPECT_TRUE(Logger::instance().hasErrors());
	Logger::instance().clear();
}

// Missing name: [označi] => error
TEST(Oznaci, MissingName) {
	moEnv env;
	Logger::instance().clear();
	auto r = eval_str("[označi]", env);
	EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
	EXPECT_TRUE(Logger::instance().hasErrors());
	Logger::instance().clear();
}
