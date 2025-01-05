#ifndef H_UTILITIES_H
#define H_UTILITIES_H

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
#include <cstring>

namespace strings {
    bool starts_with(const std::string_view &source, const std::string_view &prefix);
        const char *trimSpace(const char *str);

    

    std::string &trimSpaceLeft(std::string &str);
    std::string &trimSpaceRight(std::string &str);
    std::string_view trimSpaceLeft(std::string_view str);
    std::string_view trimSpaceRight(std::string_view str);
    inline std::string &trimSpace(std::string &s) {
        return trimSpaceRight(trimSpaceLeft(s));
    }
    inline std::string_view trimSpace(std::string_view s) {
        return trimSpaceRight(trimSpaceLeft(s));
    }

}


#endif
