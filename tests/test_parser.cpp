#include <gtest/gtest.h>

#include "rex/ast.hpp"
#include "rex/parser.hpp"

TEST(Parser, SingleCharProducesCharNode) {
  rex::NodePtr p = rex::parse("a");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, 'a');
}

TEST(Parser, TwoCharsProduceConcat) {
  rex::NodePtr p = rex::parse("ab");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, 'a');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->right->val, 'b');
}

TEST(Parser, ThreeCharsFoldLeft) {
  rex::NodePtr p = rex::parse("abc");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->left, nullptr);
  EXPECT_EQ(p->left->left->val, 'a');
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->val, 'b');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->right->val, 'c');
}

TEST(Parser, EmptyPatternFails) { EXPECT_EQ(rex::parse(""), nullptr); }

TEST(Parser, LoneStarFails) { EXPECT_EQ(rex::parse("*"), nullptr); }
