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

TEST(Parser, AltOfTwoChars) {
  rex::NodePtr p = rex::parse("a|b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, 'a');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->right->val, 'b');
}

TEST(Parser, AltFoldsLeft) {
  rex::NodePtr p = rex::parse("a|b|c");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Alt);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->val, 'c');
}

TEST(Parser, ConcatBindsTighterThanAlt) {
  rex::NodePtr p = rex::parse("ab|c");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->right->val, 'c');
}

TEST(Parser, StarWrapsChar) {
  rex::NodePtr p = rex::parse("a*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, 'a');
  EXPECT_EQ(p->right, nullptr);
}

TEST(Parser, StarBindsToLastAtomOnly) {
  rex::NodePtr p = rex::parse("ab*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, 'a');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Star);
  ASSERT_NE(p->right->left, nullptr);
  EXPECT_EQ(p->right->left->val, 'b');
}

TEST(Parser, StarThenMoreInput) {
  rex::NodePtr p = rex::parse("a*b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Star);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->val, 'b');
}

TEST(Parser, StarOverAltBranch) {
  rex::NodePtr p = rex::parse("a|b*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Star);
}

TEST(Parser, FourCharsFoldLeftDeep) {
  rex::NodePtr p = rex::parse("abcd");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->val, 'd');
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->left, nullptr);
  EXPECT_EQ(p->left->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->left->left, nullptr);
  EXPECT_EQ(p->left->left->left->val, 'a');
}

TEST(Parser, AltOfTwoConcats) {
  rex::NodePtr p = rex::parse("ab|cd");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Concat);
  ASSERT_NE(p->right->left, nullptr);
  EXPECT_EQ(p->right->left->val, 'c');
}

TEST(Parser, StarOnLeftAltBranch) {
  rex::NodePtr p = rex::parse("a*|b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Star);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->val, 'b');
}

TEST(Parser, TwoStarsSideBySide) {
  rex::NodePtr p = rex::parse("a*b*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Star);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Star);
  ASSERT_NE(p->right->left, nullptr);
  EXPECT_EQ(p->right->left->val, 'b');
}

TEST(Parser, SpaceIsLiteralChar) {
  rex::NodePtr p = rex::parse(" ");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, ' ');
}

TEST(Parser, StarNodeHasNoRightChild) {
  rex::NodePtr p = rex::parse("a*|b");
  ASSERT_NE(p, nullptr);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->right, nullptr);
}

TEST(Parser, BossTreeAltStarConcat) {
  rex::NodePtr p = rex::parse("(a|b)*c");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->right->val, 'c');
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Star);
  ASSERT_NE(p->left->left, nullptr);
  EXPECT_EQ(p->left->left->kind, rex::Kind::Alt);
  ASSERT_NE(p->left->left->left, nullptr);
  EXPECT_EQ(p->left->left->left->val, 'a');
  ASSERT_NE(p->left->left->right, nullptr);
  EXPECT_EQ(p->left->left->right->val, 'b');
}

TEST(Parser, GroupIsTransparent) {
  rex::NodePtr p = rex::parse("(a)");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, 'a');
}

TEST(Parser, NestedGroupsCollapse) {
  rex::NodePtr p = rex::parse("((a))");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, 'a');
}

TEST(Parser, StarredGroup) {
  rex::NodePtr p = rex::parse("(ab)*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
}

TEST(Parser, GroupInsideConcat) {
  rex::NodePtr p = rex::parse("a(b|c)");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->val, 'a');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Alt);
}

TEST(Parser, UnclosedGroupFails) { EXPECT_EQ(rex::parse("(a"), nullptr); }

TEST(Parser, UnclosedGroupAfterCharFails) {
  EXPECT_EQ(rex::parse("a("), nullptr);
}

TEST(Parser, UnclosedGroupWithContentAfterCharFails) {
  EXPECT_EQ(rex::parse("a(b"), nullptr);
}

TEST(Parser, EmptyGroupFails) { EXPECT_EQ(rex::parse("()"), nullptr); }

TEST(Parser, EmptyGroupAfterCharFails) {
  EXPECT_EQ(rex::parse("a()"), nullptr);
}

TEST(Parser, DoubleStarFails) { EXPECT_EQ(rex::parse("a**"), nullptr); }

TEST(Parser, LeadingStarFails) { EXPECT_EQ(rex::parse("*a"), nullptr); }

TEST(Parser, StarAfterPipeFails) { EXPECT_EQ(rex::parse("a|*b"), nullptr); }

TEST(Parser, LoneOpenParenFails) { EXPECT_EQ(rex::parse("("), nullptr); }

TEST(Parser, StrayCloseParenFails) { EXPECT_EQ(rex::parse("a)"), nullptr); }

TEST(Parser, DoublePipeFails) { EXPECT_EQ(rex::parse("a||b"), nullptr); }

TEST(Parser, TrailingPipeFails) { EXPECT_EQ(rex::parse("a|"), nullptr); }

TEST(Parser, LeadingPipeFails) { EXPECT_EQ(rex::parse("|a"), nullptr); }

TEST(Parser, EmptyPatternFails) { EXPECT_EQ(rex::parse(""), nullptr); }

TEST(Parser, LoneStarFails) { EXPECT_EQ(rex::parse("*"), nullptr); }
