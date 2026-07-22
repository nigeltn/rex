#include <gtest/gtest.h>

#include "rex/ast.hpp"
#include "rex/parser.hpp"

TEST(ParserPostfix, PlusWrapsChar) {
  rex::NodePtr p = rex::parse("a+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Plus);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, 'a');
  EXPECT_EQ(p->right, nullptr);
}

TEST(ParserPostfix, QuestWrapsChar) {
  rex::NodePtr p = rex::parse("a?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Quest);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->val, 'a');
  EXPECT_EQ(p->right, nullptr);
}

TEST(ParserPostfix, PlusBindsToLastAtom) {
  rex::NodePtr p = rex::parse("ab+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->val, 'a');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Plus);
}

TEST(ParserPostfix, QuestBindsToLastAtom) {
  rex::NodePtr p = rex::parse("ab?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Quest);
}

TEST(ParserPostfix, PlusOnGroup) {
  rex::NodePtr p = rex::parse("(ab)+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Plus);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
}

TEST(ParserPostfix, QuestOnGroup) {
  rex::NodePtr p = rex::parse("(a|b)?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Quest);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Alt);
}

TEST(ParserPostfix, PlusOnStarredGroup) {
  rex::NodePtr p = rex::parse("(a*)+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Plus);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Star);
}

TEST(ParserPostfix, MixedPostfixChain) {
  rex::NodePtr p = rex::parse("a*b+c?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Quest);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->left, nullptr);
  EXPECT_EQ(p->left->left->kind, rex::Kind::Star);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Plus);
}

TEST(ParserPostfix, PlusInLeftAltBranch) {
  rex::NodePtr p = rex::parse("a+|b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Plus);
}

TEST(ParserPostfix, QuestInRightAltBranch) {
  rex::NodePtr p = rex::parse("a|b?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Quest);
}

TEST(ParserPostfix, LeadingPlusFails) { EXPECT_EQ(rex::parse("+a"), nullptr); }

TEST(ParserPostfix, LeadingQuestFails) { EXPECT_EQ(rex::parse("?a"), nullptr); }

TEST(ParserPostfix, DoublePlusFails) { EXPECT_EQ(rex::parse("a++"), nullptr); }

TEST(ParserPostfix, DoubleQuestFails) { EXPECT_EQ(rex::parse("a??"), nullptr); }

TEST(ParserPostfix, PlusThenStarFails) { EXPECT_EQ(rex::parse("a+*"), nullptr); }

TEST(ParserPostfix, StarThenPlusFails) { EXPECT_EQ(rex::parse("a*+"), nullptr); }

TEST(ParserPostfix, QuestThenPlusFails) { EXPECT_EQ(rex::parse("a?+"), nullptr); }

TEST(ParserPostfix, StarThenQuestFails) { EXPECT_EQ(rex::parse("a*?"), nullptr); }

TEST(ParserPostfix, PipeThenPlusFails) { EXPECT_EQ(rex::parse("a|+b"), nullptr); }

TEST(ParserDot, LoneDot) {
  rex::NodePtr p = rex::parse(".");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Dot);
  EXPECT_EQ(p->left, nullptr);
  EXPECT_EQ(p->right, nullptr);
}

TEST(ParserDot, DotBetweenChars) {
  rex::NodePtr p = rex::parse("a.b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Dot);
}

TEST(ParserDot, StarOverDot) {
  rex::NodePtr p = rex::parse(".*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Dot);
}

TEST(ParserDot, PlusOverDot) {
  rex::NodePtr p = rex::parse(".+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Plus);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Dot);
}

TEST(ParserDot, DotInAltBranch) {
  rex::NodePtr p = rex::parse("a|.");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Dot);
}

TEST(ParserDot, DotInGroup) {
  rex::NodePtr p = rex::parse("(.)");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Dot);
}

TEST(ParserEscape, EscapedStar) {
  rex::NodePtr p = rex::parse("\\*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '*');
}

TEST(ParserEscape, EscapedPlus) {
  rex::NodePtr p = rex::parse("\\+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '+');
}

TEST(ParserEscape, EscapedQuest) {
  rex::NodePtr p = rex::parse("\\?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '?');
}

TEST(ParserEscape, EscapedPipe) {
  rex::NodePtr p = rex::parse("\\|");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '|');
}

TEST(ParserEscape, EscapedOpenParen) {
  rex::NodePtr p = rex::parse("\\(");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '(');
}

TEST(ParserEscape, EscapedCloseParen) {
  rex::NodePtr p = rex::parse("\\)");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, ')');
}

TEST(ParserEscape, EscapedDot) {
  rex::NodePtr p = rex::parse("\\.");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '.');
}

TEST(ParserEscape, EscapedOpenBracket) {
  rex::NodePtr p = rex::parse("\\[");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '[');
}

TEST(ParserEscape, EscapedCloseBracket) {
  rex::NodePtr p = rex::parse("\\]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, ']');
}

TEST(ParserEscape, EscapedBackslash) {
  rex::NodePtr p = rex::parse("\\\\");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, '\\');
}

TEST(ParserEscape, EscapedOrdinaryCharIsItself) {
  rex::NodePtr p = rex::parse("\\a");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Char);
  EXPECT_EQ(p->val, 'a');
}

TEST(ParserEscape, EscapedStarBetweenChars) {
  rex::NodePtr p = rex::parse("a\\*b");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->right->val, '*');
}

TEST(ParserEscape, EscapedStarIsStarrable) {
  rex::NodePtr p = rex::parse("\\**");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, '*');
}

TEST(ParserEscape, LoneBackslashFails) { EXPECT_EQ(rex::parse("\\"), nullptr); }

TEST(ParserEscape, TrailingBackslashFails) {
  EXPECT_EQ(rex::parse("a\\"), nullptr);
}

TEST(ParserClass, SimpleClass) {
  rex::NodePtr p = rex::parse("[abc]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_TRUE(p->set.test('b'));
  EXPECT_TRUE(p->set.test('c'));
  EXPECT_FALSE(p->set.test('d'));
  EXPECT_EQ(p->left, nullptr);
  EXPECT_EQ(p->right, nullptr);
}

TEST(ParserClass, SingleCharClass) {
  rex::NodePtr p = rex::parse("[a]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_FALSE(p->set.test('b'));
}

TEST(ParserClass, Range) {
  rex::NodePtr p = rex::parse("[a-z]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_TRUE(p->set.test('m'));
  EXPECT_TRUE(p->set.test('z'));
  EXPECT_FALSE(p->set.test('A'));
  EXPECT_FALSE(p->set.test('0'));
}

TEST(ParserClass, TwoRanges) {
  rex::NodePtr p = rex::parse("[a-zA-Z]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('q'));
  EXPECT_TRUE(p->set.test('Q'));
  EXPECT_FALSE(p->set.test('5'));
}

TEST(ParserClass, RangeAndSingles) {
  rex::NodePtr p = rex::parse("[a-cx]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('b'));
  EXPECT_TRUE(p->set.test('x'));
  EXPECT_FALSE(p->set.test('d'));
}

TEST(ParserClass, SingleCharRange) {
  rex::NodePtr p = rex::parse("[a-a]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_FALSE(p->set.test('b'));
}

TEST(ParserClass, Negated) {
  rex::NodePtr p = rex::parse("[^a]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_FALSE(p->set.test('a'));
  EXPECT_TRUE(p->set.test('b'));
  EXPECT_TRUE(p->set.test('^'));
}

TEST(ParserClass, NegatedRange) {
  rex::NodePtr p = rex::parse("[^a-z]");
  ASSERT_NE(p, nullptr);
  EXPECT_FALSE(p->set.test('m'));
  EXPECT_TRUE(p->set.test('M'));
}

TEST(ParserClass, CaretNotFirstIsLiteral) {
  rex::NodePtr p = rex::parse("[a^]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_TRUE(p->set.test('^'));
  EXPECT_FALSE(p->set.test('b'));
}

TEST(ParserClass, LeadingDashIsLiteral) {
  rex::NodePtr p = rex::parse("[-a]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('-'));
  EXPECT_TRUE(p->set.test('a'));
}

TEST(ParserClass, TrailingDashIsLiteral) {
  rex::NodePtr p = rex::parse("[a-]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('-'));
  EXPECT_TRUE(p->set.test('a'));
}

TEST(ParserClass, CloseBracketFirstIsLiteral) {
  rex::NodePtr p = rex::parse("[]a]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_TRUE(p->set.test(']'));
  EXPECT_TRUE(p->set.test('a'));
}

TEST(ParserClass, NegatedCloseBracketFirstIsLiteral) {
  rex::NodePtr p = rex::parse("[^]a]");
  ASSERT_NE(p, nullptr);
  EXPECT_FALSE(p->set.test(']'));
  EXPECT_FALSE(p->set.test('a'));
  EXPECT_TRUE(p->set.test('b'));
}

TEST(ParserClass, MetacharsAreLiteralInside) {
  rex::NodePtr p = rex::parse("[*+?.|()]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Class);
  EXPECT_TRUE(p->set.test('*'));
  EXPECT_TRUE(p->set.test('+'));
  EXPECT_TRUE(p->set.test('?'));
  EXPECT_TRUE(p->set.test('.'));
  EXPECT_TRUE(p->set.test('|'));
  EXPECT_TRUE(p->set.test('('));
  EXPECT_TRUE(p->set.test(')'));
}

TEST(ParserClass, EscapedCloseBracketInside) {
  rex::NodePtr p = rex::parse("[a\\]]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('a'));
  EXPECT_TRUE(p->set.test(']'));
}

TEST(ParserClass, EscapedBackslashInside) {
  rex::NodePtr p = rex::parse("[\\\\]");
  ASSERT_NE(p, nullptr);
  EXPECT_TRUE(p->set.test('\\'));
}

TEST(ParserClass, StarredClass) {
  rex::NodePtr p = rex::parse("[ab]*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Class);
}

TEST(ParserClass, PlusOnClass) {
  rex::NodePtr p = rex::parse("[0-9]+");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Plus);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Class);
  EXPECT_TRUE(p->left->set.test('7'));
}

TEST(ParserClass, ClassInConcat) {
  rex::NodePtr p = rex::parse("a[bc]d");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Class);
}

TEST(ParserClass, ClassInAltBranch) {
  rex::NodePtr p = rex::parse("a|[bc]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Alt);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Class);
}

TEST(ParserClass, EmptyClassFails) { EXPECT_EQ(rex::parse("[]"), nullptr); }

TEST(ParserClass, UnclosedClassFails) { EXPECT_EQ(rex::parse("[a"), nullptr); }

TEST(ParserClass, UnclosedAfterCharFails) {
  EXPECT_EQ(rex::parse("x[a"), nullptr);
}

TEST(ParserClass, UnclosedRangeFails) { EXPECT_EQ(rex::parse("[a-"), nullptr); }

TEST(ParserClass, ReversedRangeFails) {
  EXPECT_EQ(rex::parse("[z-a]"), nullptr);
}

TEST(ParserClass, LoneOpenBracketFails) { EXPECT_EQ(rex::parse("["), nullptr); }

TEST(ParserMix, GroupedAltWithClassAndPostfix) {
  rex::NodePtr p = rex::parse("(a|[0-9]+)?");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Quest);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Alt);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Plus);
  ASSERT_NE(p->left->right->left, nullptr);
  EXPECT_EQ(p->left->right->left->kind, rex::Kind::Class);
}

TEST(ParserMix, KitchenSinkSpine) {
  rex::NodePtr p = rex::parse("(a|[0-9]+)?.b*");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Star);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Concat);
  ASSERT_NE(p->left->right, nullptr);
  EXPECT_EQ(p->left->right->kind, rex::Kind::Dot);
  ASSERT_NE(p->left->left, nullptr);
  EXPECT_EQ(p->left->left->kind, rex::Kind::Quest);
}

TEST(ParserMix, EscapeDotClassTogether) {
  rex::NodePtr p = rex::parse("\\.[.]");
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(p->kind, rex::Kind::Concat);
  ASSERT_NE(p->left, nullptr);
  EXPECT_EQ(p->left->kind, rex::Kind::Char);
  EXPECT_EQ(p->left->val, '.');
  ASSERT_NE(p->right, nullptr);
  EXPECT_EQ(p->right->kind, rex::Kind::Class);
  EXPECT_TRUE(p->right->set.test('.'));
}
