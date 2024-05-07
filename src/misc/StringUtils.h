#pragma once
#include <string>
#include <sstream>

namespace StringUtils {
    /**
     * @brief Provides a way to get a String representation for variables or objects
     * @tparam T - Type of object/variable.
     * @param t - Object to stringify. If a custom object, should override the << operator.
     * @return String representation of `t`
     */
    template <typename T>
    std::string toString(const T& t) {
        std::ostringstream s;
        s << t;
        return s.str();
    }
}
