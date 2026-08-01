#include "ast/expression.hpp"

#include "ast/if.hpp"
#include "ast/fn_call.hpp"
#include "ast/literal.hpp"


namespace ast {


Expression::Expression(const Expression& other) {
    assign_from(other);
}

Expression& Expression::operator=(const Expression& other) {
    delete_data();
    assign_from(other);

    return *this;
}

void Expression::assign_from(const Expression& other) {

    // Initialize union members explicitely to avoid any UB
    if (other.holds<If>()) {
        ptr._if = new If { *other.ptr._if };
        tag = if_tag;
    }
    else if (other.holds<FnCall>()) {
        ptr.call = new FnCall { *other.ptr.call };
        tag = call_tag;
    }
    else if (other.holds<Literal>()) {
        ptr.lit = new Literal { *other.ptr.lit };
        tag = lit_tag;
    }
    else if (other.holds<ScopedIdentifier>()) {
        ptr.id = new ScopedIdentifier { *other.ptr.id };
        tag = id_tag;
    }
    
}

void Expression::delete_data() {
    if (holds<If>()) {
        delete ptr._if;
    }
    else if (holds<FnCall>()) {
        delete ptr.call;
    }
    else if (holds<Literal>()) {
        delete ptr.lit;
    }
    else if (holds<ScopedIdentifier>()) {
        delete ptr.id;
    }
}

inline Expression::~Expression() {
    delete_data();
}

inline Expression::Expression(If&& _if) :
    ptr { ._if = new If { std::move(_if) } },
    tag { if_tag } { }

inline Expression::Expression(FnCall&& call) :
    ptr { .call = new FnCall { std::move(call) } },
    tag { call_tag } { }

inline Expression::Expression(Literal&& lit) :
    ptr { .lit = new Literal { std::move(lit) } },
    tag { lit_tag } { }

inline Expression::Expression(ScopedIdentifier&& id) :
    ptr { .id = new ScopedIdentifier { std::move(id) } },
    tag { id_tag } { }


}  // namespace ast
