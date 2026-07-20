#include <gtest/gtest.h>
#include "rex/ast.hpp"
#include "rex/parser.hpp"

TEST(Parser, SingleCharProducesCharNode) {
    rex::NodePtr p = rex::parse("a");
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->kind, rex::Kind::Char);
    EXPECT_EQ(p->val, 'a');
}