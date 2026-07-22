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
  if (ch == '|' || ch == '*' || ch == '(' || ch == ')') {
    return false;
  }
  return true;
}

static NodePtr atom(std::string_view& pattern) {
  if (!starts_atom(pattern)) {
    return nullptr;
  }
  char ch = pattern.front();
  pattern.remove_prefix(1);
  return std::make_unique<Node>(Kind::Char, ch);
}

static NodePtr concat(std::string_view& pattern) {
  NodePtr curr = atom(pattern);
  if (curr == nullptr) {
    return nullptr;
  }
  while (starts_atom(pattern)) {
    NodePtr ch = atom(pattern);
    curr =
        std::make_unique<Node>(Kind::Concat, 0, std::move(curr), std::move(ch));
  }
  return curr;
}

NodePtr parse(std::string_view pattern) {
  NodePtr tree = concat(pattern);
  if (tree == nullptr || !pattern.empty()) {
    return nullptr;
  }
  return tree;
}
}  // namespace rex