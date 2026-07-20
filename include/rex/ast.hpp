#pragma once
#include <memory>
namespace rex {
    enum class Kind {
        Char
    };

    struct Node {
        Kind kind;
        char val = 0;
    };

    using NodePtr = std::unique_ptr<Node>;
}