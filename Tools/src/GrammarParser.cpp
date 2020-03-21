// Name   : GrammarParser.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

#include <fstream>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>

// Read the grammar from file.
// The function could generate the TerminalSet and NonTerminalSet.
// The TerminalSet generated will contains '$', which is used to compute FirstSet.
std::vector<std::shared_ptr<Production>> 
read_grammar(const std::string &filename, const std::string &null) {
    std::vector<std::shared_ptr<Production>> prodVec;
    std::ifstream is(filename);
    if (!is) {
        std::cerr << RED << "Failed to open file '" << filename << "'." << NONE 
                  << std::endl;
        return prodVec;
    }
    std::string line;
    while (getline(is, line)) {
        auto tmpVec = split(line, "->");
        std::string left = trim(tmpVec[0]);
        std::vector<std::string> rights;
        tmpVec = split(trim(tmpVec[1]), " ");
        for (int i = 0; i < tmpVec.size(); ++i)
            rights.push_back(trim(tmpVec[i]));
        prodVec.push_back(std::make_shared<Production>(left, rights));
    }
    is.close();
    Production::setStart(prodVec[0]->left); // Set the `Start` symbol.
    Production::setNull(null);              // Set the `null` symbol.
    return prodVec;
}
