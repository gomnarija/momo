#include <gtest/gtest.h>
#include "momo.h"

using namespace momo;

// --- string_to_number (string overload) ---

// Valid integer string
TEST(Utils, StringToNumberInteger) {
	EXPECT_DOUBLE_EQ(string_to_number("42"), 42.0);
}

// Valid decimal string
TEST(Utils, StringToNumberDecimal) {
	EXPECT_DOUBLE_EQ(string_to_number("3.14"), 3.14);
}

// Negative number
TEST(Utils, StringToNumberNegative) {
	EXPECT_DOUBLE_EQ(string_to_number("-7"), -7.0);
}

// Invalid string returns 0 (and logs an error)
TEST(Utils, StringToNumberInvalid) {
	Logger::instance().clear();
	double result = string_to_number("abc");
	EXPECT_DOUBLE_EQ(result, 0.0);
	// Should have logged an error
	EXPECT_TRUE(Logger::instance().hasErrors());
	Logger::instance().clear();
}

// --- string_remove_quotes ---

// Removes surrounding double quotes
TEST(Utils, RemoveQuotesNormal) {
	EXPECT_EQ(string_remove_quotes("\"hello\""), "hello");
}

// Returns unchanged if no quotes
TEST(Utils, RemoveQuotesNoQuotes) {
	EXPECT_EQ(string_remove_quotes("hello"), "hello");
}

// Only opening quote — no removal
TEST(Utils, RemoveQuotesOnlyOpening) {
	EXPECT_EQ(string_remove_quotes("\"hello"), "\"hello");
}

// Empty string stays empty
TEST(Utils, RemoveQuotesEmpty) {
	EXPECT_EQ(string_remove_quotes(""), "");
}

// --- get_file_name ---

// Extracts filename from Unix path
TEST(Utils, GetFileNameUnix) {
	std::string result = get_file_name("some/path/to/file.momo");
	EXPECT_EQ(result, "/file.momo");
}

// Extracts filename from Windows path
TEST(Utils, GetFileNameWindows) {
	std::string result = get_file_name("C:\\Users\\file.momo");
	EXPECT_EQ(result, "\\file.momo");
}

// No directory — returns the path itself
TEST(Utils, GetFileNameNoDir) {
	EXPECT_EQ(get_file_name("file.momo"), "file.momo");
}
