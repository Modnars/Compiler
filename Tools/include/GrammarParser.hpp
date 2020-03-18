// Name   : GrammarParser.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_PARSER_HPP
#define __GRAMMAR_PARSER_HPP

#include <string>

#include "GrammarAnalysis.hpp"

extern std::vector<std::shared_ptr<Production>> 
read_grammar(const std::string &f, const std::string &null = "$");

namespace LR {
    extern bool analyze(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os = std::cout);
}

namespace SLR {
    extern bool analyze(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os = std::cout);
}

#endif /* __GRAMMAR_PARSER_HPP */
