#ifndef AST_EMPTY_HPP
#define AST_EMPTY_HPP


namespace ast {


class Empty {
public:
    Empty() noexcept = default;
    Empty(Empty&& other) noexcept = default;
    Empty& operator=(const Empty& other) = default;
    Empty& operator=(Empty&& other) noexcept = default;
};


}


#endif  // AST_EMPTY_HPP
