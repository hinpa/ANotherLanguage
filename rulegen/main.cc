#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>



void CreateVisitorClass(std::ostream &out, std::string fileName, std::string visitorName, std::string retType) {
    std::ifstream file(fileName);
    std::string s;
    std::vector<std::string> classNames;
    while (std::getline(file,s))
    {
        if (s.empty())
            continue;
        std::stringstream ss(s);
        std::string className, visitorType;
        std::getline(ss,className,':');
        std::getline(ss,visitorType,':');
        std::getline(ss,visitorType,';');
        if (visitorType == visitorName) {
            classNames.push_back(className);
            out << "struct " << className << ";\n";
        }
    }
    out << "struct " << visitorName << " {\n";
    for (auto i : classNames) {
        out << "    virtual " << retType << " visit" << i << "(" << i << "&) = 0;\n";
    }
    out << "};\n";
}

void CreateHeader(std::ostream& out, std::string className, std::string parent) {
    out << "struct " << className;
    if (parent.size()) {
        out << " : public " << parent;
    }
    out << " {\n";
}

void createMember(std::ostream &out, std::string type, std::string declarator, std::string baseValue) {
    out << "    " << type << " " << declarator;
    if (baseValue.size())
        out << " = " << baseValue;
    out << ";\n";
}


bool isEmpty(const std::string &s)
{
    for (auto c : s)
    {
        if (!isspace(c))
            return false;
    }
    return true;
}

std::string onlyName(const std::string &declarator) {
    std::string ret;
    for (auto c : declarator) {
        if (isalnum(c) || c == '_')
            ret += c;
    }
    return ret;
}

int main(int argc, char **argv) {
    if (argc < 2)
    {
        std::cout << "Usage: ./astgen <file> [<ofile>] [<prepend>]* -- [<append>]*\n";
        return EXIT_FAILURE;
    }
    std::ofstream ofile;
    if (argc >= 3)
        ofile.open(argv[2]);

    std::ostream &handle = ofile.is_open() ? ofile : std::cout;

    int i = 3;
    if (argc >= 4)
    {
        while (i != argc)
        {
            if (std::string(argv[i]) == "--") {
                ++i;
                break;
            }
            std::ifstream file(argv[i]);
            char contents[1024];
            while (file.read(contents, 1024))
            {
                handle << contents;
            }
            contents[file.gcount()] = '\0';
            handle << contents;
            ++i;
        }
        handle << "\n\n";
    }

    CreateVisitorClass(handle, argv[1], "IExprVisitor", "ExpressionResult");
    CreateVisitorClass(handle, argv[1], "IStmtVisitor", "void");

    std::map<std::string, std::string> visitors = {
        {"IExprVisitor", "ExpressionResult"},
        {"IStmtVisitor", "void"}
    };

    std::ifstream ifile(argv[1]);
    std::string line;
    std::string lastInheritsFrom;
    while (std::getline(ifile,line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string statement;
        std::getline(ss, statement, ';');
        std::istringstream temp(statement);
        std::string className, inheritsFrom, visitorName;
        std::getline(temp, className, ':');
        std::getline(temp, inheritsFrom, ':');
        std::getline(temp, visitorName, ';');
        CreateHeader(handle, className, inheritsFrom);
        std::vector<std::tuple<std::string, std::string, bool> > members;
        if (inheritsFrom != lastInheritsFrom) {
            members.clear();
            lastInheritsFrom = inheritsFrom;
        }
        while (std::getline(ss, statement, ';')) {
            if (isEmpty(statement))
                continue;
            std::string baseType, declarator, baseVal;
            std::istringstream temp(statement);
            temp >> baseType;
            if (baseType == "const")
            {
                temp >> baseType;
                baseType = "const " + baseType;
            }
            temp >> declarator >> baseVal;
            bool isrval = baseVal == "RVAL";
            if (isrval)
                baseVal = "";
            createMember(handle, baseType, declarator, baseVal);
            if (!isrval) {
                temp >> baseVal;
                isrval = baseVal == "RVAL";
            }

            members.push_back({baseType, declarator, isrval});
        }
        handle << "    " << className << "(";
        for (size_t i = 0; i != members.size(); ++i) { 
            if (i != 0)
                handle << ", ";
            std::string baseType;
            std::string declarator;
            bool isrval;
            std::tie(baseType, declarator, isrval) = members[i];
            if (isrval)
                baseType += "&&";
            handle << baseType << " " << declarator << "_";
        }
        handle << ") : ";

        for (size_t i = 0; i != members.size(); ++i) { 
            if (i != 0)
                handle << ", ";
            std::string baseType;
            std::string declarator;
            bool isrval;
            std::tie(baseType, declarator, isrval) = members[i];
            declarator = onlyName(declarator);
            handle << declarator << "(";
            declarator.push_back('_');
            if (isrval)
                declarator = "std::move(" + declarator + ")";
            handle << declarator << ") "; 
        }
        handle << " {} \n";
        handle << "    virtual " << visitors[visitorName] << " accept(" << visitorName << " *v) { return v->visit" << className << "(*this); }\n";
        handle << "~" << className << "(){};\n";
        handle << "};\n\n";
    }
    if (i != argc) {
        while (i != argc) {
            std::ifstream file(argv[i]);
            char contents[1024];
            while (file.read(contents, 1024))
            {
                handle << contents;
            }
            contents[file.gcount()] = '\0';
            handle << contents;
            ++i;
        }
    }
    handle << "\n#endif";
    
}
