#include <core/gtype.h>
#include <cmath>

namespace core {
    core::gtype get_value(const std::string& s)
    {
        if (s == "true" || s == "false")
        {
            return s == "true" ? true : false;
        }
        else if (isdigit(s[0]))
        {
            return std::stof(s);
        }
        else if (s[0] == '"' && s[s.size() - 1] == '"')
        {
            return s.substr(1, s.size() - 2);
        }

        return std::monostate{};
    }

    std::string stringify(const gtype& value)
    {
        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "nil"; // or "null"
            } else if constexpr (std::is_same_v<T, double>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return v;
            } else if constexpr (std::is_same_v<T, bool>) {
                return v ? "true" : "false";
            } else {
                return "unknown"; // fallback
            }
        }, value);
    }

    bool equals(const gtype& a, const gtype& b)
    {
        if (a.index() != b.index()) return false; // different types

        return std::visit([](auto&& x, auto&& y) -> bool {
            using X = std::decay_t<decltype(x)>;
            using Y = std::decay_t<decltype(y)>;

            if constexpr (std::is_same_v<X, Y>) {
                if constexpr (std::is_same_v<X, double>) {
                    return std::fabs(x - y) < 1e-9; // handle float equality
                } else {
                    return x == y;
                }
            } else {
                return false;
            }
        }, a, b);
    }
}