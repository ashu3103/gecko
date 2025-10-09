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
    /* Comparision */
    bool equals(const gtype& a, const gtype& b);
}

#endif // __GTYPE_H__