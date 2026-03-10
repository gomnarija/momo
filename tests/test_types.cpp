#include <gtest/gtest.h>
#include "momo.h"

using namespace momo;

// --- moNumber ---

// Default-constructed number should be 0
TEST(MoNumber, DefaultIsZero) {
	moNumber n;
	EXPECT_EQ(n.getValue(), 0.0);
}

// Construct from double
TEST(MoNumber, ConstructFromDouble) {
	moNumber n(42.5);
	EXPECT_DOUBLE_EQ(n.getValue(), 42.5);
}

// Construct from string
TEST(MoNumber, ConstructFromString) {
	moNumber n("3.14");
	EXPECT_DOUBLE_EQ(n.getValue(), 3.14);
}

// Type tag
TEST(MoNumber, TypeIsMoNumber) {
	moNumber n(1.0);
	EXPECT_EQ(n.getType(), MO_TYPE::MO_NUMBER);
}

// Zero is falsy, nonzero is truthy
TEST(MoNumber, Truthiness) {
	moNumber zero(0.0);
	moNumber nonzero(1.0);
	EXPECT_FALSE(zero.isTrue());
	EXPECT_TRUE(nonzero.isTrue());
}

// Equal numbers compare equal
TEST(MoNumber, EqualNumbers) {
	auto a = std::make_shared<moNumber>(5.0);
	auto b = std::make_shared<moNumber>(5.0);
	EXPECT_TRUE(a->equals(b));
}

// Different numbers are not equal
TEST(MoNumber, DifferentNumbers) {
	auto a = std::make_shared<moNumber>(5.0);
	auto b = std::make_shared<moNumber>(6.0);
	EXPECT_FALSE(a->equals(b));
}

// Number is not equal to a string with same printed value
TEST(MoNumber, NotEqualToString) {
	auto n = std::make_shared<moNumber>(5.0);
	auto s = std::make_shared<moString>("5.000000");
	EXPECT_FALSE(n->equals(s));
}

// --- moString ---

// Construct from string
TEST(MoString, ConstructAndGetValue) {
	moString s("hello");
	EXPECT_EQ(s.getValue(), "hello");
}

// print() returns the raw value (no quotes)
TEST(MoString, PrintReturnsRaw) {
	moString s("world");
	EXPECT_EQ(s.print(), "world");
}

// Type tag
TEST(MoString, TypeIsMoString) {
	moString s("x");
	EXPECT_EQ(s.getType(), MO_TYPE::MO_STRING);
}

// Empty string is falsy, non-empty is truthy
TEST(MoString, Truthiness) {
	moString empty("");
	moString nonempty("a");
	EXPECT_FALSE(empty.isTrue());
	EXPECT_TRUE(nonempty.isTrue());
}

// Equal strings
TEST(MoString, EqualStrings) {
	auto a = std::make_shared<moString>("abc");
	auto b = std::make_shared<moString>("abc");
	EXPECT_TRUE(a->equals(b));
}

// Different strings
TEST(MoString, DifferentStrings) {
	auto a = std::make_shared<moString>("abc");
	auto b = std::make_shared<moString>("xyz");
	EXPECT_FALSE(a->equals(b));
}

// --- moSymbol ---

// Construct and print
TEST(MoSymbol, ConstructAndPrint) {
	moSymbol s("foo");
	EXPECT_EQ(s.print(), "foo");
}

// Type tag
TEST(MoSymbol, TypeIsMoSymbol) {
	moSymbol s("bar");
	EXPECT_EQ(s.getType(), MO_TYPE::MO_SYMBOL);
}

// --- moList ---

// Empty list
TEST(MoList, EmptyList) {
	moList l;
	EXPECT_EQ(l.size(), 0u);
	EXPECT_EQ(l.print(), "()");
}

// Insert and access
TEST(MoList, InsertAndAccess) {
	moList l;
	l.insert(std::make_shared<moNumber>(1.0));
	l.insert(std::make_shared<moNumber>(2.0));
	EXPECT_EQ(l.size(), 2u);
	EXPECT_EQ(l.at(0)->print(), "1.000000");
	EXPECT_EQ(l.at(1)->print(), "2.000000");
}

// Out-of-bounds returns NIL
TEST(MoList, OutOfBoundsReturnsNil) {
	moList l;
	EXPECT_EQ(l.at(0)->getType(), MO_TYPE::MO_NIL);
	EXPECT_EQ(l.at(999)->getType(), MO_TYPE::MO_NIL);
}

// Type tag
TEST(MoList, TypeIsMoList) {
	moList l;
	EXPECT_EQ(l.getType(), MO_TYPE::MO_LIST);
}

// Empty list is falsy, non-empty is truthy
TEST(MoList, Truthiness) {
	moList empty;
	moList nonempty;
	nonempty.insert(std::make_shared<moNumber>(1.0));
	EXPECT_FALSE(empty.isTrue());
	EXPECT_TRUE(nonempty.isTrue());
}

// Construct from vector
TEST(MoList, ConstructFromVector) {
	std::vector<moValPtr> items = {
		std::make_shared<moNumber>(1.0),
		std::make_shared<moString>("hi")
	};
	moList l(items);
	EXPECT_EQ(l.size(), 2u);
	EXPECT_EQ(l.at(0)->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_EQ(l.at(1)->getType(), MO_TYPE::MO_STRING);
}

// --- moFunction ---

// Construct and inspect (moFunction is abstract — use raw new via moFunctionPtr)
TEST(MoFunction, ConstructAndInspect) {
	auto args = std::make_shared<moList>();
	args->insert(std::make_shared<moSymbol>("x"));

	auto body = std::make_shared<moList>();
	body->insert(std::make_shared<moSymbol>("x"));

	moFunctionPtr f(new moFunction("add", args, body));
	EXPECT_EQ(f->getName(), "add");
	EXPECT_EQ(f->getArgs()->size(), 1u);
	EXPECT_EQ(f->getBody()->size(), 1u);
	EXPECT_EQ(f->getType(), MO_TYPE::MO_FUNCTION);
}

// print() returns function name
TEST(MoFunction, PrintReturnsName) {
	auto args = std::make_shared<moList>();
	auto body = std::make_shared<moList>();
	moFunctionPtr f(new moFunction("myFunc", args, body));
	EXPECT_EQ(f->print(), "myFunc");
}

// --- _moNil ---

// NIL singleton behavior
TEST(MoNil, PrintReturnsNista) {
	EXPECT_EQ(NIL->print(), "Ništa");
}

TEST(MoNil, TypeIsMoNil) {
	EXPECT_EQ(NIL->getType(), MO_TYPE::MO_NIL);
}

// NIL is falsy
TEST(MoNil, IsFalsy) {
	EXPECT_FALSE(NIL->isTrue());
}

// NIL equals another NIL
TEST(MoNil, NilEqualsNil) {
	_moNil a;
	auto other = std::make_shared<_moNil>();
	EXPECT_TRUE(a.equals(other));
}

// NIL does not equal a number
TEST(MoNil, NilNotEqualToNumber) {
	_moNil a;
	auto n = std::make_shared<moNumber>(0.0);
	EXPECT_FALSE(a.equals(n));
}
