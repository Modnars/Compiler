// Name   : GrammarParser.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_PARSER_HPP
#define __GRAMMAR_PARSER_HPP

#include <string>

extern int read_grammar(const std::string &filename);

namespace LR {
    extern void analysis(std::ostream &os = std::cout);
}

namespace SLR {
    extern void analysis(std::ostream &os = std::cout);
}

#endif /* __GRAMMAR_PARSER_HPP */
