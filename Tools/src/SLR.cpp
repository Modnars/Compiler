// Name   : LR.cpp
// Author : Modnar
// Date   : 2019-09-27
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <fstream>
#include <stack>
#include <cstdlib>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

namespace {
    // Store the Production sequence.
    std::vector<std::shared_ptr<Production>> ProdVec; 
    // Store the non-terminal symbols.
    std::set<std::string> NonTerminalSet;             
    // Store the terminal symbols.
    std::set<std::string> TerminalSet;                
}

namespace SLR {
    void initialize(const std::vector<std::shared_ptr<Production>> &prods) {
        ProdVec = prods;
        for (auto prod : ProdVec)
            if (!contains(NonTerminalSet, prod->left)) 
                NonTerminalSet.insert(prod->left);
        for (auto prod : ProdVec)
            for (auto right : prod->rights)
                if (!contains(NonTerminalSet, right))
                    TerminalSet.insert(right);
    }

    void analysis(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os) {
        initialize(prods);
        for (auto prod: ProdVec)
            std::cout << *prod << std::endl;
        for (auto sym : NonTerminalSet)
            std::cout << sym << std::endl;
        for (auto sym : TerminalSet)
            std::cout << sym << std::endl;
    }
}
