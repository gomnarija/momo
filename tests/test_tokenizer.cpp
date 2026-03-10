#include <gtest/gtest.h>
#include "tokenizer.h"

using namespace momo;

// Tokenizing a simple number
TEST(Tokenizer, SingleNumber) {
	std::string input = "42";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::NUMBER);
	EXPECT_EQ(t.curr(), "42");
}

// Tokenizing a decimal number
TEST(Tokenizer, DecimalNumber) {
	std::string input = "3.14";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::NUMBER);
	EXPECT_EQ(t.curr(), "3.14");
}

// Tokenizing a string literal (quoted)
TEST(Tokenizer, StringLiteral) {
	std::string input = "\"hello world\"";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::STRING);
	EXPECT_EQ(t.curr(), "\"hello world\"");
}

// Tokenizing a symbol
TEST(Tokenizer, Symbol) {
	std::string input = "foo";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::SYMBOL);
	EXPECT_EQ(t.curr(), "foo");
}

// Left bracket
TEST(Tokenizer, LeftBracket) {
	std::string input = "[";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::L_PARAN);
	EXPECT_EQ(t.curr(), "[");
}

// Right bracket
TEST(Tokenizer, RightBracket) {
	std::string input = "]";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::R_PARAN);
	EXPECT_EQ(t.curr(), "]");
}

// Whitespace is a token
TEST(Tokenizer, Whitespace) {
	std::string input = "  ";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::WHITESPACE);
}

// Sequence: [+ 1 2]
TEST(Tokenizer, SimpleExpression) {
	std::string input = "[+ 1 2]";
	Tokenizer t(input);

	// [
	EXPECT_EQ(t.getState(), TokenizerState::L_PARAN);
	t.next();

	// +
	EXPECT_EQ(t.getState(), TokenizerState::SYMBOL);
	EXPECT_EQ(t.curr(), "+");
	t.next();

	// whitespace
	EXPECT_EQ(t.getState(), TokenizerState::WHITESPACE);
	t.next();

	// 1
	EXPECT_EQ(t.getState(), TokenizerState::NUMBER);
	EXPECT_EQ(t.curr(), "1");
	t.next();

	// whitespace
	EXPECT_EQ(t.getState(), TokenizerState::WHITESPACE);
	t.next();

	// 2
	EXPECT_EQ(t.getState(), TokenizerState::NUMBER);
	EXPECT_EQ(t.curr(), "2");
	t.next();

	// ]
	EXPECT_EQ(t.getState(), TokenizerState::R_PARAN);
}

// isEnd() returns true after consuming all tokens
TEST(Tokenizer, IsEndAfterAll) {
	std::string input = "x";
	Tokenizer t(input);
	t.next();
	EXPECT_TRUE(t.isEnd());
}

// Empty input goes straight to END
TEST(Tokenizer, EmptyInput) {
	std::string input = "";
	Tokenizer t(input);
	EXPECT_TRUE(t.isEnd());
}

// setInputString resets the tokenizer
TEST(Tokenizer, SetInputStringResets) {
	std::string input = "abc";
	Tokenizer t(input);
	EXPECT_EQ(t.curr(), "abc");

	t.setInputString("123");
	EXPECT_EQ(t.getState(), TokenizerState::NUMBER);
	EXPECT_EQ(t.curr(), "123");
}

// Semicolon token
TEST(Tokenizer, Semicolon) {
	std::string input = ";";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::SEMICOLON);
}

// Hyphen-prefixed symbol (e.g. operator -)
TEST(Tokenizer, HyphenSymbol) {
	std::string input = "-";
	Tokenizer t(input);
	EXPECT_EQ(t.getState(), TokenizerState::SYMBOL);
	EXPECT_EQ(t.curr(), "-");
}

// TODO: colon-prefixed tokens for Phase 2 (:key desugaring)
// TEST(Tokenizer, ColonPrefix) { ... }

// TODO: dot-containing symbols for Phase 4 (property access)
// TEST(Tokenizer, DotSymbol) { ... }
