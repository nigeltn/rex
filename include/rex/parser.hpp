#pragma once
#include <string_view>

#include "rex/ast.hpp"

namespace rex {
NodePtr parse(std::string_view);
}