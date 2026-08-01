#ifndef AST_DATA_TYPE_HPP
#define AST_DATA_TYPE_HPP


#include "ast/scoped_identifier.hpp"


namespace ast {


class DataType {
public:

    enum Mut {
        Mutable,
        Const
    };

    enum Ref {
        Reference,
        Pointer,
        Value
    };

private:

    ScopedIdentifier type_name;
    Mut mut;
    Ref ref;

public:

    DataType(const DataType& other) = default;
    DataType(DataType&& other) noexcept = default;

    DataType& operator=(const DataType& other) = default;
    DataType& operator=(DataType&& other) noexcept = default;

    const ScopedIdentifier& type() const {
        return type_name;
    }

    Mut mutability() const { return mut; }
    bool is_mutable() const { return mut == Mut::Mutable; }
    bool is_const() const { return mut == Mut::Const; }

    Ref reference_status() const { return ref; }
    bool is_ref() const { return ref == Ref::Reference; }
    bool is_pointer() const { return ref == Ref::Pointer; }
    bool is_value() const { return ref == Ref::Value; }
    bool is_referential() const { return not is_value(); }
};


}  // namespace ast


#endif  // AST_DATA_TYPE_HPP
