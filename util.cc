#include "util.hpp"

namespace strings {


    bool starts_with(const std::string_view &source, const std::string_view &prefix) {
        auto sourceI = source.begin();
        auto prefixI = prefix.begin();
        while (sourceI != source.end() && prefixI != prefix.end())
        {
            if (*sourceI != *prefixI)
                return false;
            ++sourceI, ++prefixI;
        }
        return prefixI == prefix.end();
    }

    const char *trimSpace(const char *str) {
        while (isspace(*str))
        {
            ++str;
        }
        return str;
    }

    

    std::string_view trimSpaceLeft(std::string_view str)
    {
        if (str.empty())
            return "";
        size_t i = 0;
        while (i < str.size() && isspace(str[i]))
        {
            ++i;
        }
        if (i == str.size()) return "";
        return str.substr(i, str.size());
    }
    std::string &trimSpaceLeft(std::string &str)
    {
        if (str.empty())
            return str;
        size_t i = 0;
        while (i < str.size() && isspace(str[i]))
        {
            ++i;
        }
        if (i == str.size())
        {
            str = "";
            return str;
        }
        str = str.substr(i,str.size());
        return str;
    }
    std::string &trimSpaceRight(std::string &str) {
        if (str.empty())
            return str;
        size_t i = str.size() - 1;
        while (i > 0 && isspace(str[i])) {
            --i;
        }
        if (str[i] == ' ')
        {
            str = "";
            return str;
        }
        str = str.substr(0, i + 1);
        return str;
    }
    std::string_view trimSpaceRight(std::string_view str) {
        if (str.empty())
            return str;
        size_t i = str.size() - 1;
        while (i > 0 && isspace(str[i])) {
            --i;
        }
        if (str[i] == ' ')
        {
            return "";
        }
        return str.substr(0, i + 1);
    }

}

