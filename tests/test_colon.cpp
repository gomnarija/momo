#include <gtest/gtest.h>
#include "momo.h"
#include "tokenizer.h"
#include "reader.h"

using namespace momo;

static moValPtr eval_str(const std::string& code, moEnv& env) {
	std::string input = code;
	moValPtr ast = readString(input);
	return eval(ast, env);
}

// --- Tokenizer: colon is a standalone token ---

// `:` by itself
TEST(ColonTokenizer, StandaloneColon) {
	std::string input = ":";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::COLON);
	EXPECT_EQ(t.curr(), ":");
}

// `:muda` splits into COLON + SYMBOL
TEST(ColonTokenizer, ColonNoSpace) {
	std::string input = ":muda";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::COLON);
	EXPECT_EQ(t.curr(), ":");
	t.next();
	EXPECT_EQ(t.getState(), TokenizerState::SYMBOL);
	EXPECT_EQ(t.curr(), "muda");
}

// `: muda` splits into COLON + WHITESPACE + SYMBOL
TEST(ColonTokenizer, ColonWithSpace) {
	std::string input = ": muda";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::COLON);
	t.next();
	EXPECT_EQ(t.getState(), TokenizerState::WHITESPACE);
	t.next();
	EXPECT_EQ(t.getState(), TokenizerState::SYMBOL);
	EXPECT_EQ(t.curr(), "muda");
}

// --- Reader: colon desugaring ---

// `:boja "crvena"` at top level => [označi boja "crvena"]
TEST(ColonReader, TopLevelNoSpace) {
	std::string input = ":boja \"crvena\"";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_LIST);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	// First element is symbol "označi"
	EXPECT_EQ(list->at(0)->getType(), MO_TYPE::MO_SYMBOL);
	EXPECT_EQ(list->at(0)->print(), "označi");
	// Second is the name
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_SYMBOL);
	EXPECT_EQ(list->at(1)->print(), "boja");
	// Third is the value
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_STRING);
	EXPECT_EQ(list->at(2)->print(), "crvena");
}

// `: boja "crvena"` with space => same result
TEST(ColonReader, TopLevelWithSpace) {
	std::string input = ": boja \"crvena\"";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	EXPECT_EQ(list->at(0)->print(), "označi");
	EXPECT_EQ(list->at(1)->print(), "boja");
	EXPECT_EQ(list->at(2)->print(), "crvena");
}

// Colon with number value: `:x 42`
TEST(ColonReader, WithNumberValue) {
	std::string input = ":x 42";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	EXPECT_EQ(list->at(1)->print(), "x");
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_NUMBER);
}

// Colon with list value: `:tip ["a" "b"]`
TEST(ColonReader, WithListValue) {
	std::string input = ":tip [\"a\" \"b\"]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_LIST);
}

// Colon inside a list: [f :x 1] => [f [označi x 1]]
TEST(ColonReader, InsideList) {
	std::string input = "[f :x 1]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 2u);
	// f
	EXPECT_EQ(list->at(0)->print(), "f");
	// [označi x 1]
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_LIST);
	auto inner = std::static_pointer_cast<moList>(list->at(1));
	EXPECT_EQ(inner->size(), 3u);
	EXPECT_EQ(inner->at(0)->print(), "označi");
	EXPECT_EQ(inner->at(1)->print(), "x");
	EXPECT_EQ(inner->at(2)->getType(), MO_TYPE::MO_NUMBER);
}

// Multiple colons in a list: [f :a 1 :b 2]
TEST(ColonReader, MultipleInsideList) {
	std::string input = "[f :a 1 :b 2]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	// [označi a 1]
	auto first = std::static_pointer_cast<moList>(list->at(1));
	EXPECT_EQ(first->at(0)->print(), "označi");
	EXPECT_EQ(first->at(1)->print(), "a");
	// [označi b 2]
	auto second = std::static_pointer_cast<moList>(list->at(2));
	EXPECT_EQ(second->at(0)->print(), "označi");
	EXPECT_EQ(second->at(1)->print(), "b");
}

// Colon with space inside list: [f : x 1]
TEST(ColonReader, InsideListWithSpace) {
	std::string input = "[f : x 1]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 2u);
	auto inner = std::static_pointer_cast<moList>(list->at(1));
	EXPECT_EQ(inner->at(0)->print(), "označi");
	EXPECT_EQ(inner->at(1)->print(), "x");
}

// --- End-to-end: colon + eval ---

// `:x 5` evaluates to bind x = 5
TEST(ColonEval, BindsVariable) {
	moEnv env;
	eval_str(":x 5", env);
	auto r = eval_str("x", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 5.0);
}

// `: name "momo"` with space
TEST(ColonEval, BindsWithSpace) {
	moEnv env;
	eval_str(": name \"momo\"", env);
	auto r = eval_str("name", env);
	EXPECT_EQ(r->print(), "momo");
}

// Colon inside list that gets evaluated
TEST(ColonEval, InsideEvaluatedList) {
	moEnv env;
	// This creates a list [označi x [+ 1 2]] and evals the outer expression
	// But the outer list has operator "f" which doesn't exist...
	// Better test: use označi directly to show it works in context
	eval_str("[označi a 10]", env);
	eval_str(":b [+ a 5]", env);
	auto r = eval_str("b", env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 15.0);
}
