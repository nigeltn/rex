#pragma once
#include "rex/ast.hpp"
#include <string_view>

namespace rex {
    NodePtr parse(std::string_view);
}