#pragma once
#include <bitset>
#include <memory>
namespace rex {
enum class Kind { Char, Alt, Concat, Star, Plus, Quest, Dot, Class };

struct Node;
using NodePtr = std::unique_ptr<Node>;

struct Node {
  Kind kind;
  char val = 0;
  NodePtr left, right;
  std::bitset<256> allowed_set;
};
}  // namespace rex