// Name   : GrammarParser.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_PARSER_HPP
#define __GRAMMAR_PARSER_HPP

#include <memory>
#include <string>

#include "GrammarAnalysis.hpp"

namespace Parser {
// Modified by `main`, which is used for controlling whether show execution details.
extern bool SHOW_DETAILS;
} // namespace Parser

extern std::vector<std::shared_ptr<Production>> read_grammar(const std::string &f,
                                                             const std::string &null = std::string("$"));

namespace LL {
extern bool analyze(const std::vector<std::shared_ptr<Production>> &prods, std::ostream &os = std::cout);
}

namespace SLR {
extern bool analyze(const std::vector<std::shared_ptr<Production>> &prods, std::ostream &os = std::cout);
}

namespace LR {
extern bool analyze(const std::vector<std::shared_ptr<Production>> &prods, std::ostream &os = std::cout);
}

#endif /* __GRAMMAR_PARSER_HPP */
