#include <gtest/gtest.h>
#include <momo.h>
#include <reader.h>

using namespace momo;

// helper: parse and eval a string
static moValPtr run(const std::string& code, moEnv& env){
    std::string s = code;
    moValPtr ast = readString(s);
    return eval(ast, env);
}


// --- basic creation ---

// [funkcija [x] x] → moFunction
TEST(Funkcija, CreatesFunction){
    moEnv env;
    moValPtr r = run("[funkcija [x] x]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_FUNCTION);
}

// [funkcija [] 42] → moFunction with no args
TEST(Funkcija, NoArgs){
    moEnv env;
    moValPtr r = run("[funkcija [] 42]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_FUNCTION);
}

// [funkcija [a b] [+ a b]] → moFunction with two args
TEST(Funkcija, MultipleArgs){
    moEnv env;
    moValPtr r = run("[funkcija [a b] [+ a b]]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_FUNCTION);
    auto fn = std::static_pointer_cast<moFunction>(r);
    EXPECT_EQ(fn->getArgs()->size(), 2u);
}


// --- calling functions ---

// :f [funkcija [x] [+ x 1]]  [f 5] → 6
TEST(Funkcija, BasicCall){
    moEnv env;
    run(":f [funkcija [x] [+ x 1]]", env);
    moValPtr r = run("[f 5]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 6.0);
}

// :add [funkcija [a b] [+ a b]]  [add 3 4] → 7
TEST(Funkcija, TwoArgCall){
    moEnv env;
    run(":add [funkcija [a b] [+ a b]]", env);
    moValPtr r = run("[add 3 4]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 7.0);
}

// :nula [funkcija [] 0]  [nula] → 0
TEST(Funkcija, ZeroArgCall){
    moEnv env;
    run(":nula [funkcija [] 0]", env);
    moValPtr r = run("[nula]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 0.0);
}


// --- multiple body expressions (returns last) ---

// :f [funkcija [x] [:a 1] [+ x a]]  [f 10] → 11
TEST(Funkcija, MultipleBodyReturnsLast){
    moEnv env;
    run(":f [funkcija [x] [:a 1] [+ x a]]", env);
    moValPtr r = run("[f 10]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 11.0);
}


// --- lexical scoping ---

// :x 100  :f [funkcija [] x]  [f] → 100
TEST(Funkcija, CapturesOuterVariable){
    moEnv env;
    run(":x 100", env);
    run(":f [funkcija [] x]", env);
    moValPtr r = run("[f]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 100.0);
}

// function args shadow outer variables
// :x 5  :f [funkcija [x] x]  [f 99] → 99
TEST(Funkcija, ArgsShadowOuter){
    moEnv env;
    run(":x 5", env);
    run(":f [funkcija [x] x]", env);
    moValPtr r = run("[f 99]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 99.0);
}

// function body doesn't leak bindings
// :f [funkcija [] [:lokalno 42]]  [f]  lokalno → NIL
TEST(Funkcija, BodyDoesntLeakBindings){
    moEnv env;
    run(":f [funkcija [] [:lokalno 42]]", env);
    run("[f]", env);
    moValPtr r = run("lokalno", env);
    EXPECT_EQ(r->getType(), MO_TYPE::MO_NIL);
}


// --- recursion ---

// :fakt [funkcija [n] [ako [= n 0] 1 [* n [fakt [- n 1]]]]]
// [fakt 5] → 120
TEST(Funkcija, Recursion){
    moEnv env;
    run(":fakt [funkcija [n] [ako [= n 0] 1 [* n [fakt [- n 1]]]]]", env);
    moValPtr r = run("[fakt 5]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 120.0);
}


// --- higher-order functions ---

// :primeni [funkcija [f x] [f x]]
// :duplo [funkcija [n] [* n 2]]
// [primeni duplo 5] → 10
TEST(Funkcija, HigherOrder){
    moEnv env;
    run(":primeni [funkcija [f x] [f x]]", env);
    run(":duplo [funkcija [n] [* n 2]]", env);
    moValPtr r = run("[primeni duplo 5]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 10.0);
}


// --- function with ako ---

// :aps [funkcija [n] [ako [< n 0] [* n [- 0 1]] n]]
// [aps [- 0 5]] → 5  [aps 3] → 3
TEST(Funkcija, WithAko){
    moEnv env;
    run(":aps [funkcija [n] [ako [< n 0] [* n [- 0 1]] n]]", env);
    moValPtr r1 = run("[aps [- 0 5]]", env);
    ASSERT_EQ(r1->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r1)->getValue(), 5.0);
    moValPtr r2 = run("[aps 3]", env);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r2)->getValue(), 3.0);
}


// --- function returns function ---

// :napravi-dodavac [funkcija [n] [funkcija [x] [+ x n]]]
// :dodaj5 [napravi-dodavac 5]
// [dodaj5 10] → 15
TEST(Funkcija, ReturnsFunction){
    moEnv env;
    run(":napravi-dodavac [funkcija [n] [funkcija [x] [+ x n]]]", env);
    run(":dodaj5 [napravi-dodavac 5]", env);
    moValPtr r = run("[dodaj5 10]", env);
    ASSERT_EQ(r->getType(), MO_TYPE::MO_NUMBER);
    EXPECT_DOUBLE_EQ(std::static_pointer_cast<moNumber>(r)->getValue(), 15.0);
}
