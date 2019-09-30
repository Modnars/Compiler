// Name   : LR.cpp
// Author : Modnar
// Date   : 2019-09-27
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <map>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

namespace {
    // Store the Production sequence.
    std::vector<std::shared_ptr<Production>> ProdVec; 
    // Store the non-terminal symbols.
    std::set<std::string> NonTerminalSet;             
    // Store the terminal symbols.
    std::set<std::string> TerminalSet;                

    // Store the First Set.
    std::map<std::string, std::shared_ptr<std::vector<std::string>>> FirstSet;  
    // Store the Follow Set.
    std::map<std::string, std::shared_ptr<std::vector<std::string>>> FollowSet; 

    // Store the Closures' set.
    std::vector<std::vector<Item>> ClosureSet; 
    // Store the Action information of Reduce, Shift and Goto.
    std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;
    
    std::stack<int> StateStack;  // Store the State Stack information.
    std::stack<std::string> SymbolStack; // Store the Symbol Stack information.
}

namespace SLR {
    void initialize(const std::vector<std::shared_ptr<Production>> &prods) {
        ProdVec = prods;
//        for (std::size_t i = 0; i < ProdVec.size(); ++i) {
//            if (i > 10) break;
//            std::cout << ProdVec[i] << " " << prods[i] << std::endl;
//        }
        for (auto prod : ProdVec)
            if (!contains(NonTerminalSet, prod->left)) 
                NonTerminalSet.insert(prod->left);
        for (auto prod : ProdVec)
            for (auto right : prod->rights)
                if (!contains(NonTerminalSet, right))
                    TerminalSet.insert(right);
    }

    // Judge whether the production could be null directly.
    bool could_be_null(const std::string &prod) {
        for (auto pptr : ProdVec) 
            if (pptr->rights[0] == Production::null())
                return true;
        return false;
    }

    void getFirstSet() {
        std::shared_ptr<std::vector<std::string>> first;
        for (auto var : TerminalSet) { // Terminal symbol's first set is itself.
            FirstSet[var] = std::make_shared<std::vector<std::string>>();
            FirstSet[var]->push_back(var);
        }
        for (auto var : NonTerminalSet) // Initialize the non-terminal symbol's first set.
            FirstSet[var] = std::make_shared<std::vector<std::string>>();
        bool extending = true;
        while (true) {
            extending = false;
            for (auto pptr : ProdVec) {
                auto left = pptr->left;
                for (auto right : pptr->rights) {
                    if (right != Production::null()) {
                        for (auto new_sym : *FirstSet[right])
                            if (!contains(*FirstSet[left], new_sym)) {
                                FirstSet[left]->push_back(new_sym);
                                extending = true;
                            }
                    }
                    if (!could_be_null(right))
                        break;
                }
            }
            if (!extending)
                break;
        }
    }

    // Get the FOLLOW Set of the grammar.
    void getFollowSet() {
        getFirstSet();
        for (auto var : NonTerminalSet) 
            FollowSet[var] = std::make_shared<std::vector<std::string>>();
        FollowSet["S"]->push_back("#");
        bool flag, fab;
        while (true) {
            flag = true;
            for (int i = 0; i < ProdVec.size(); ++i) {
                std::string left, right;
                std::vector<std::string> rights = ProdVec[i]->rights;
                for (int j = 0; j < rights.size(); ++j) {
                    right = rights[j];
                    if (contains(NonTerminalSet, right)) {
                        fab = true;
                        for (int k = j+1; k < rights.size(); ++k) {
                            for (int v = 0; v < FirstSet[rights[k]]->size(); ++v) {
                                if (contains(*FollowSet[right], (*FirstSet[rights[k]])[v])) {
                                    continue;
                                } else {
                                    FollowSet[right]->push_back((*FirstSet[rights[k]])[v]);
                                    flag = false;
                                }
                            } /* for (int v ...) loop */
                            if (could_be_null(rights[k])) {
                                continue;
                            } else {
                                fab = false;
                                break;
                            }
                        } /* for (int k ...) loop */
                        if (fab) {
                            left = ProdVec[i]->left;
                            for (int p = 0; p < FollowSet[left]->size(); ++p) {
                                if (contains(*FollowSet[right], (*FollowSet[left])[p])) {
                                    continue;
                                } else {
                                    FollowSet[right]->push_back((*FollowSet[left])[p]);
                                    flag = false;
                                }
                            } /* for (int p ...) loop */
                        } /* if (fab) */
                    } /* if (contains(...)) */
                } /* for (int j ...) loop */
            } /* for (int i ...) loop */
            if (flag) break;
        } /* while (true) */
    }

    void analyze(const std::vector<std::shared_ptr<Production>> &prods, 
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
