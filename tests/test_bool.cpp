#include <gtest/gtest.h>
#include "momo.h"
#include "reader.h"

using namespace momo;

// --- moBool type ---

// tačno has correct type
TEST(MoBool, TacnoType) {
	moBool b(true);
	EXPECT_EQ(b.getType(), MO_TYPE::MO_BOOL);
}

// netačno has correct type
TEST(MoBool, NetacnoType) {
	moBool b(false);
	EXPECT_EQ(b.getType(), MO_TYPE::MO_BOOL);
}

// tačno prints as "tačno"
TEST(MoBool, TacnoPrint) {
	moBool b(true);
	EXPECT_EQ(b.print(), "tačno");
}

// netačno prints as "netačno"
TEST(MoBool, NetacnoPrint) {
	moBool b(false);
	EXPECT_EQ(b.print(), "netačno");
}

// tačno is truthy
TEST(MoBool, TacnoIsTruthy) {
	moBool b(true);
	EXPECT_TRUE(b.isTrue());
}

// netačno is falsy
TEST(MoBool, NetacnoIsFalsy) {
	moBool b(false);
	EXPECT_FALSE(b.isTrue());
}

// getValue returns the underlying bool
TEST(MoBool, GetValue) {
	moBool t(true);
	moBool f(false);
	EXPECT_TRUE(t.getValue());
	EXPECT_FALSE(f.getValue());
}

// tačno equals tačno
TEST(MoBool, TacnoEqualsTacno) {
	auto a = std::make_shared<moBool>(true);
	auto b = std::make_shared<moBool>(true);
	EXPECT_TRUE(a->equals(b));
}

// netačno equals netačno
TEST(MoBool, NetacnoEqualsNetacno) {
	auto a = std::make_shared<moBool>(false);
	auto b = std::make_shared<moBool>(false);
	EXPECT_TRUE(a->equals(b));
}

// tačno does not equal netačno
TEST(MoBool, TacnoNotEqualsNetacno) {
	auto a = std::make_shared<moBool>(true);
	auto b = std::make_shared<moBool>(false);
	EXPECT_FALSE(a->equals(b));
}

// bool does not equal a number
TEST(MoBool, NotEqualToNumber) {
	auto b = std::make_shared<moBool>(true);
	auto n = std::make_shared<moNumber>(1.0);
	EXPECT_FALSE(b->equals(n));
}

// bool does not equal NIL
TEST(MoBool, NotEqualToNil) {
	auto b = std::make_shared<moBool>(false);
	EXPECT_FALSE(b->equals(NIL));
}

// --- Reader: boolean literals ---

// "tačno" is read as a bool, not a symbol
TEST(ReaderBool, ReadTacno) {
	std::string input = "tačno";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(val->isTrue());
}

// "netačno" is read as a bool, not a symbol
TEST(ReaderBool, ReadNetacno) {
	std::string input = "netačno";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(val->isTrue());
}

// Boolean inside a list: [+ tačno netačno]
TEST(ReaderBool, BoolInList) {
	std::string input = "[foo tačno netačno]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	// foo is still a symbol
	EXPECT_EQ(list->at(0)->getType(), MO_TYPE::MO_SYMBOL);
	// tačno is a bool
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(list->at(1)->isTrue());
	// netačno is a bool
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(list->at(2)->isTrue());
}

// --- Eval: booleans self-evaluate ---

// tačno evaluates to itself
TEST(EvalBool, TacnoSelfEval) {
	moEnv env;
	std::string input = "tačno";
	moValPtr ast = readString(input);
	moValPtr result = eval(ast, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_BOOL);
	EXPECT_TRUE(result->isTrue());
}

// netačno evaluates to itself
TEST(EvalBool, NetacnoSelfEval) {
	moEnv env;
	std::string input = "netačno";
	moValPtr ast = readString(input);
	moValPtr result = eval(ast, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_BOOL);
	EXPECT_FALSE(result->isTrue());
}
