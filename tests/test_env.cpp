#include <gtest/gtest.h>
#include "momo.h"

using namespace momo;

// Fresh env returns NIL for missing key
TEST(MoEnv, MissingKeyReturnsNil) {
	moEnv env;
	moValPtr val = env.getVal("x");
	EXPECT_EQ(val->getType(), MO_TYPE::MO_NIL);
}

// insertVal adds a new binding
TEST(MoEnv, InsertAndGet) {
	moEnv env;
	auto num = std::make_shared<moNumber>(42.0);
	env.insertVal("x", num);
	moValPtr val = env.getVal("x");
	EXPECT_EQ(val->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(val)->getValue(), 42.0);
}

// insertVal does NOT overwrite existing key
TEST(MoEnv, InsertDoesNotOverwrite) {
	moEnv env;
	env.insertVal("x", std::make_shared<moNumber>(1.0));
	env.insertVal("x", std::make_shared<moNumber>(2.0));
	// Should still be 1.0
	double val = std::static_pointer_cast<moNumber>(env.getVal("x"))->getValue();
	EXPECT_DOUBLE_EQ(val, 1.0);
}

// setVal only updates existing key
TEST(MoEnv, SetValUpdatesExisting) {
	moEnv env;
	env.insertVal("x", std::make_shared<moNumber>(1.0));
	env.setVal("x", std::make_shared<moNumber>(99.0));
	double val = std::static_pointer_cast<moNumber>(env.getVal("x"))->getValue();
	EXPECT_DOUBLE_EQ(val, 99.0);
}

// setVal does nothing for non-existing key
TEST(MoEnv, SetValIgnoresNonExisting) {
	moEnv env;
	env.setVal("x", std::make_shared<moNumber>(5.0));
	// x was never inserted, so it should still be NIL
	EXPECT_EQ(env.getVal("x")->getType(), MO_TYPE::MO_NIL);
}

// Child env can see parent bindings
TEST(MoEnv, ChildSeesParent) {
	moEnv parent;
	parent.insertVal("x", std::make_shared<moNumber>(10.0));
	moEnv child(&parent);
	// getVal only checks current env — env_search in eval walks the chain
	// But let's verify getUpperEnv works
	EXPECT_NE(child.getUpperEnv(), nullptr);
	EXPECT_EQ(child.getUpperEnv()->getVal("x")->getType(), MO_TYPE::MO_NUMBER);
}

// Child env shadows parent binding
TEST(MoEnv, ChildShadowsParent) {
	moEnv parent;
	parent.insertVal("x", std::make_shared<moNumber>(1.0));
	moEnv child(&parent);
	child.insertVal("x", std::make_shared<moNumber>(2.0));
	// Child has its own x
	double childVal = std::static_pointer_cast<moNumber>(child.getVal("x"))->getValue();
	EXPECT_DOUBLE_EQ(childVal, 2.0);
	// Parent is unchanged
	double parentVal = std::static_pointer_cast<moNumber>(parent.getVal("x"))->getValue();
	EXPECT_DOUBLE_EQ(parentVal, 1.0);
}

// Root env has nullptr upper
TEST(MoEnv, RootHasNullUpper) {
	moEnv env;
	EXPECT_EQ(env.getUpperEnv(), nullptr);
}

// Construct with initial bindings
TEST(MoEnv, ConstructWithMap) {
	std::map<std::string, moValPtr> bindings;
	bindings["a"] = std::make_shared<moNumber>(1.0);
	bindings["b"] = std::make_shared<moString>("hi");
	moEnv env(nullptr, bindings);
	EXPECT_EQ(env.getVal("a")->getType(), MO_TYPE::MO_NUMBER);
	EXPECT_EQ(env.getVal("b")->getType(), MO_TYPE::MO_STRING);
}

// TODO: Phase 3 — označi should overwrite (new behavior, different from insertVal)
