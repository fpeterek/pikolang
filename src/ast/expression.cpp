#include "ast/expression.hpp"

#include "ast/if.hpp"
#include "ast/fn_call.hpp"
#include "ast/literal.hpp"
#include "ast/body.hpp"


namespace ast {

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
    else if (holds<Body>()) {
        delete ptr.body;
    }
}

Expression::~Expression() {
    delete_data();
}

Expression::Expression(If&& _if) :
    ptr { ._if = new If { std::move(_if) } },
    tag { if_tag } { }

Expression::Expression(FnCall&& call) :
    ptr { .call = new FnCall { std::move(call) } },
    tag { call_tag } { }

Expression::Expression(Literal&& lit) :
    ptr { .lit = new Literal { std::move(lit) } },
    tag { lit_tag } { }

Expression::Expression(ScopedIdentifier&& id) :
    ptr { .id = new ScopedIdentifier { std::move(id) } },
    tag { id_tag } { }

Expression::Expression(Body&& body) :
    ptr { .body = new Body { std::move(body) } },
    tag { id_tag } { }

}  // namespace ast
