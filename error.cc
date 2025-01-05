#include "error.hpp"
#include <algorithm>
#include <string>
#include "util.hpp"

namespace PrettyError {
    const char *source = NULL;

    std::string prettifyError(IThrowError &e) {
        if (!source)
            return "";
        const std::string delimiter = " | ";
        std::string prettyError;
        const char *i = source;
        const char *prevNewline = source;
        const char *lastNewline = source;
        const char *end = source + e.getPos();
        size_t lineCnt = 1;
        std::string lineCntStr;
        while (*i && i != end) {
            if (*i == '\n') {
                prevNewline = lastNewline;
                lastNewline = i;
                ++lineCnt; 
            }
            ++i;
        }

        if (!*i && lastNewline + 1 == i)
        {
            i = lastNewline;
            end = lastNewline;
            lastNewline = prevNewline;
            --lineCnt;
        }
        if (lastNewline != source)
            ++lastNewline;
        while (*end && *end != '\n')
            ++end;

        size_t relPos = i - lastNewline + 1;
        lineCntStr = std::to_string(lineCnt) + ':' + std::to_string(relPos);
        std::string_view line = std::string_view(lastNewline).substr(0, end - lastNewline);
        prettyError += std::string(e.what()) + '\n';
        prettyError += lineCntStr + delimiter + std::string(line) + '\n';
        prettyError += std::string(lineCntStr.size() + delimiter.size() + relPos, ' ');
        prettyError.back() = '^';

        return prettyError;

    }
}
