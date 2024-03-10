#pragma once
#include <string>
#include <sstream>

namespace StringUtils {
    template <typename T>
    std::string toString(const T& t) {
        std::ostringstream s;
        s << t;
        return s.str();
    }
}
