// Name   : GrammarParser.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_PARSER_HPP
#define __GRAMMAR_PARSER_HPP

#include <string>

#include "GrammarAnalysis.hpp"

// extern int read_grammar(const std::string &filename);
extern std::vector<std::shared_ptr<Production>> read_grammar(const std::string &f);

namespace LR {
    extern void analyze(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os = std::cout);
}

namespace SLR {
    extern void analyze(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os = std::cout);
}

#endif /* __GRAMMAR_PARSER_HPP */
