#include "rex/parser.hpp"

#include <cstddef>
#include <memory>
#include <string_view>

#include "rex/ast.hpp"

namespace rex {
static bool starts_atom(std::string_view pattern) {
  if (pattern.empty()) {
    return false;
  }
  char ch = pattern.front();
  if (ch == '|' || ch == '*' || ch == ')') {
    return false;
  }
  return true;
}

static NodePtr alternation(std::string_view&);

static NodePtr atom(std::string_view& pattern) {
  if (!starts_atom(pattern)) {
    return nullptr;
  }
  char ch = pattern.front();
  pattern.remove_prefix(1);
  if (ch != '(') {
    return std::make_unique<Node>(Kind::Char, ch);
  }
  NodePtr new_branch = alternation(pattern);
  if (new_branch == nullptr) {
    return nullptr;
  }
  if (pattern.empty() || (!pattern.empty() && pattern.front() != ')')) {
    return nullptr;
  }
  pattern.remove_prefix(1);
  return new_branch;
}

static NodePtr repeat(std::string_view& pattern) {
  NodePtr ch = atom(pattern);
  if (ch == nullptr) {
    return nullptr;
  }
  if (!pattern.empty() && pattern.front() == '*') {
    pattern.remove_prefix(1);
    return std::make_unique<Node>(Kind::Star, 0, std::move(ch), nullptr);
  }
  return ch;
}

static NodePtr concat(std::string_view& pattern) {
  NodePtr curr = repeat(pattern);
  if (curr == nullptr) {
    return nullptr;
  }
  while (starts_atom(pattern)) {
    NodePtr ch = repeat(pattern);
    if (ch == nullptr) {
      return nullptr;
    }
    curr =
        std::make_unique<Node>(Kind::Concat, 0, std::move(curr), std::move(ch));
  }
  return curr;
}

static NodePtr alternation(std::string_view& pattern) {
  NodePtr curr = concat(pattern);
  if (curr == nullptr) {
    return nullptr;
  }
  while (!pattern.empty() && pattern.front() == '|') {
    pattern.remove_prefix(1);
    NodePtr new_branch = concat(pattern);
    if (new_branch == nullptr) {
      return nullptr;
    }
    curr = std::make_unique<Node>(Kind::Alt, 0, std::move(curr),
                                  std::move(new_branch));
  }
  return curr;
}

NodePtr parse(std::string_view pattern) {
  NodePtr tree = alternation(pattern);
  if (tree == nullptr || !pattern.empty()) {
    return nullptr;
  }
  return tree;
}
}  // namespace rex