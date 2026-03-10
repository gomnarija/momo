#include <gtest/gtest.h>
#include "momo.h"
#include "reader.h"

using namespace momo;

// Reading a single number from string
TEST(Reader, ReadNumber) {
	std::string input = "42";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(val)->getValue(), 42.0);
}

// Reading a decimal number
TEST(Reader, ReadDecimalNumber) {
	std::string input = "3.14";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(val)->getValue(), 3.14);
}

// Reading a string literal
TEST(Reader, ReadString) {
	std::string input = "\"hello\"";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_STRING);
	// Quotes are stripped by the reader
	EXPECT_EQ(std::static_pointer_cast<moString>(val)->getValue(), "hello");
}

// Reading a symbol
TEST(Reader, ReadSymbol) {
	std::string input = "foo";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_SYMBOL);
	EXPECT_EQ(val->print(), "foo");
}

// Reading a simple list: [+ 1 2]
TEST(Reader, ReadSimpleList) {
	std::string input = "[+ 1 2]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_LIST);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	// First item is the operator symbol
	EXPECT_EQ(list->at(0)->getType(), MO_TYPE::MO_SYMBOL);
	EXPECT_EQ(list->at(0)->print(), "+");
	// Arguments are numbers
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_NUMBER);
}

// Reading an empty list: []
TEST(Reader, ReadEmptyList) {
	std::string input = "[]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(val->getType(), MO_TYPE::MO_LIST);
	EXPECT_EQ(std::static_pointer_cast<moList>(val)->size(), 0u);
}

// Nested lists: [+ [- 3 1] 2]
TEST(Reader, ReadNestedList) {
	std::string input = "[+ [- 3 1] 2]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	// Second element is a nested list
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_LIST);
	auto inner = std::static_pointer_cast<moList>(list->at(1));
	EXPECT_EQ(inner->size(), 3u);
	EXPECT_EQ(inner->at(0)->print(), "-");
}

// Reading a string with spaces inside quotes
TEST(Reader, ReadStringWithSpaces) {
	std::string input = "\"hello world\"";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	EXPECT_EQ(std::static_pointer_cast<moString>(val)->getValue(), "hello world");
}

// Mixed types in a list: [foo 42 "bar"]
TEST(Reader, ReadMixedList) {
	std::string input = "[foo 42 \"bar\"]";
	moValPtr val = readString(input);
	ASSERT_NE(val, nullptr);
	auto list = std::static_pointer_cast<moList>(val);
	EXPECT_EQ(list->size(), 3u);
	EXPECT_EQ(list->at(0)->getType(), MO_TYPE::MO_SYMBOL);
	EXPECT_EQ(list->at(1)->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_EQ(list->at(2)->getType(), MO_TYPE::MO_STRING);
}

// TODO: Phase 2 — colon desugaring
// TEST(Reader, ColonDesugar) {
//     // `:boja "crvena"` should produce [označi boja "crvena"]
// }

// TODO: Phase 2 — colon desugaring inside a list
// TEST(Reader, ColonDesugarInList) {
//     // `[f :x 1]` should produce [f [označi x 1]]
// }
