#ifndef __GTYPE_H__
#define __GTYPE_H__

#include <iostream>
#include <variant>
#include <string>
#include <compare>

namespace core {
    /* define a manual class to store primitives, used for modelling literal AST node */
    using gtype = std::variant<std::monostate, double, std::string, bool>;

    /* Convert gtype to human readable format */
    std::string stringify(const gtype& value);
    /* Convert string to gtype */
    gtype get_value(const std::string& s);
    /* compare types */
    template <typename T, typename... Ts>
    bool is_type(const std::variant<Ts...>& v);
    /* Comparision */
    bool equals(const gtype& a, const gtype& b);
}

#endif // __GTYPE_H__