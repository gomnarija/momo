#include <gtest/gtest.h>
#include "momo.h"

using namespace momo;

// --- mo_sum ---

// Sum of two numbers
TEST(Core, SumTwoNumbers) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moNumber>(3.0));
	args->insert(std::make_shared<moNumber>(7.0));
	moValPtr result = mo_sum(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 10.0);
}

// Sum with a string convertible to number
TEST(Core, SumNumberAndStringNumber) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moNumber>(5.0));
	args->insert(std::make_shared<moString>("3"));
	moValPtr result = mo_sum(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 8.0);
}

// Sum with no arguments returns NIL (error)
TEST(Core, SumNoArgsReturnsNil) {
	moEnv env;
	auto args = std::make_shared<moList>();
	moValPtr result = mo_sum(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NIL);
}

// String concatenation when first arg is string
TEST(Core, SumStringConcat) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moString>("hello"));
	args->insert(std::make_shared<moString>(" world"));
	moValPtr result = mo_sum(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_STRING);
	EXPECT_EQ(result->print(), "hello world");
}

// String concat with a number uses its print value
TEST(Core, SumStringConcatWithNumber) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moString>("val: "));
	args->insert(std::make_shared<moNumber>(5.0));
	moValPtr result = mo_sum(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_STRING);
	// moNumber::print() returns "5.000000"
	EXPECT_EQ(result->print(), "val: 5.000000");
}

// --- mo_difference ---

// Difference of two numbers
TEST(Core, DifferenceTwoNumbers) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moNumber>(10.0));
	args->insert(std::make_shared<moNumber>(3.0));
	moValPtr result = mo_difference(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 7.0);
}

// Difference with multiple arguments: 20 - 5 - 3 = 12
TEST(Core, DifferenceMultipleNumbers) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moNumber>(20.0));
	args->insert(std::make_shared<moNumber>(5.0));
	args->insert(std::make_shared<moNumber>(3.0));
	moValPtr result = mo_difference(args, env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 12.0);
}

// Difference with no args returns NIL
TEST(Core, DifferenceNoArgsReturnsNil) {
	moEnv env;
	auto args = std::make_shared<moList>();
	moValPtr result = mo_difference(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NIL);
}

// Difference with string first arg returns NIL (error)
TEST(Core, DifferenceStringFirstArgReturnsNil) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moString>("hello"));
	args->insert(std::make_shared<moNumber>(1.0));
	moValPtr result = mo_difference(args, env);
	EXPECT_EQ(result->getType(), MO_TYPE::MO_NIL);
}

// Difference with string arg that converts to number
TEST(Core, DifferenceWithStringNumber) {
	moEnv env;
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moNumber>(10.0));
	args->insert(std::make_shared<moString>("4"));
	moValPtr result = mo_difference(args, env);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(result)->getValue(), 6.0);
}

// TODO: Phase 3 — mo_product, mo_quotient
// TODO: Phase 3 — comparison operators
