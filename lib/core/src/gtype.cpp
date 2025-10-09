#include <gtype.h>
#include <cmath>

namespace core {
    std::string stringify(const gtype& a)
    {
        // TODO(ashu3103)
        return "";
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