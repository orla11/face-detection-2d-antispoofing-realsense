#ifndef _PARSER_H
#define _PARSER_H

#include "../lib/cxxopts.hpp"
#include <string>

using namespace std;

class Parser {
    private:
        cxxopts::Options parser;
        static const string PROGRAM_DESCRIPTION;
        static const string MODULE_NAME;

    public:
        Parser();
        Parser(const string moduleName, const string programDescription);

        void initDefaultOptions();
        cxxopts::ParseResult parse(int& argc, char** argv);
};

#endif