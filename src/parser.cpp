#include "rex/parser.hpp"
#include "rex/ast.hpp"
#include <memory>

namespace rex {
    NodePtr parse(std::string_view pattern) {
        return std::make_unique<Node>(Kind::Char, pattern.front());
    }
}