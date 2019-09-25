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

std::vector<std::shared_ptr<Production>> ProdVec; // Store the Production sequence.
std::set<std::string> NonTerminalSet;             // Store the non-terminal symbols.
std::set<std::string> TerminalSet;                // Store the terminal symbols.

// Store the First Set.
std::map<std::string, std::shared_ptr<std::vector<std::string>>> FirstSet;  
// std::vector<std::set<Item>> ClosureSet; // Store the Closures' set.

// Store the Action information of Reduce, Shift and Goto.
std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;

std::stack<int> StateStack;  // Store the State Stack information.
std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

// Read the grammar from file.
// The function could generate the TerminalSet and NonTerminalSet.
// The TerminalSet generated will contains '$', which is used to compute FirstSet.
int read_grammar(const std::string &filename) {
    std::ifstream is(filename);
    if (!is) {
        std::cout << RED;
        std::cerr << "Failed to open file '" << filename << "'." << std::endl;
        std::cout << NONE;
        return EXIT_FAILURE;
    }
    std::string line;
    while (getline(is, line)) {
        auto tmp_vec = split(line, "->");
        std::string left = trim(tmp_vec[0]);
        if (!contains(NonTerminalSet, left))
            NonTerminalSet.insert(left);
        std::vector<std::string> rights;
        tmp_vec = split(trim(tmp_vec[1]), " ");
        for (int i = 0; i < tmp_vec.size(); ++i)
            rights.push_back(trim(tmp_vec[i]));
        ProdVec.push_back(std::make_shared<Production>(left, rights));
    }
    is.close();
    for (auto p : ProdVec) 
        for (auto sym : p->rights) 
            if (!contains(NonTerminalSet, sym)) 
                TerminalSet.insert(sym);
    return EXIT_SUCCESS;
}