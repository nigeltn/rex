#pragma once
#include <memory>
namespace rex {
enum class Kind { Char, Alt, Concat, Star };

struct Node;
using NodePtr = std::unique_ptr<Node>;

struct Node {
  Kind kind;
  char val = 0;
  NodePtr left, right;
};
}  // namespace rex