// Name   : LL.cpp
// Author : Modnar
// Date   : 2020/03/19
// Copyright (c) 2020 Modnar. All rights reserved.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>

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
std::vector<std::set<Item>> ClosureSet;

// Store the action information of REDUCE, SHIFT and GOTO.
std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;

// Store the State Stack information.
std::stack<int> StateStack;
// Store the Symbol Stack information.
std::stack<std::string> SymbolStack;

bool failure;
} // namespace

namespace LL {
// Judge whether the symbol could be null symbol.
// For example: `A -> $` and then we see A is `could_be_null`.
bool could_be_null(const std::string &sym) {
    for (int i = 0; i != ProdVec.size(); ++i) {
        if (ProdVec[i]->left == sym && ProdVec[i]->rights[0] == Production::null()) {
            return true;
        }
    }
    return false;
}

// Compute the `FIRST` set.
void getFirstSet() {
    std::shared_ptr<std::vector<std::string>> first;
    for (auto var : TerminalSet) {
        FirstSet[var] = std::make_shared<std::vector<std::string>>();
        FirstSet[var]->push_back(var);
    }
    for (auto var : NonTerminalSet)
        FirstSet[var] = std::make_shared<std::vector<std::string>>();
    bool need_break = false;
    while (true) {
        need_break = true;
        for (int i = 0; i != ProdVec.size(); ++i) {
            std::string left = ProdVec[i]->left;
            for (int j = 0; j != ProdVec[i]->rights.size(); ++j) {
                std::string right = ProdVec[i]->rights[j];
                if (right != Production::null()) {
                    for (int k = 0; k != FirstSet[right]->size(); ++k) {
                        if (contains(*FirstSet[left], (*FirstSet[right])[k])) {
                            continue;
                        } else {
                            FirstSet[left]->push_back((*FirstSet[right])[k]);
                            need_break = false;
                        }
                    } // for (int k; ... ) ...loop
                }     // if (right != Production::null()) ...
                if (could_be_null(right)) {
                    continue;
                } else {
                    break;
                }
            } // for (int j; ... ) ...loop
        }     // for (int i; ... ) ...loop
        if (need_break)
            break;
    } // while (true) ...loop
}

void getFollowSet() {
    for (std::string sym : NonTerminalSet) {
        FollowSet[sym] = std::make_shared<std::vector<std::string>>();
    }
    FollowSet[Production::start()]->emplace_back("#");
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
                    for (int k = j + 1; k < rights.size(); ++k) {
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
                    }     /* if (fab) */
                }         /* if (contains(...)) */
            }             /* for (int j ...) loop */
        }                 /* for (int i ...) loop */
        if (flag)
            break;
    } /* while (true) */
    // Remove all the "#" from FollowSet.
    for (const std::string &left : NonTerminalSet)
        for (auto iter = FollowSet[left]->begin(); iter != FollowSet[left]->end();)
            if (*iter == "#")
                iter = FollowSet[left]->erase(iter);
            else
                ++iter;
}

void initialize(const std::vector<std::shared_ptr<Production>> &prods) {
    ProdVec = prods;
    for (auto prod : ProdVec)
        if (!contains(NonTerminalSet, prod->left))
            NonTerminalSet.insert(prod->left);
    for (auto prod : ProdVec)
        for (auto right : prod->rights)
            if (!contains(NonTerminalSet, right))
                TerminalSet.insert(right);
    getFirstSet();
    getFollowSet();
}

bool analyze(const std::vector<std::shared_ptr<Production>> &prods, std::ostream &os) {
    failure = true;
    std::cerr << "SORRY! The `LL(1)` analysis method is still under development." << std::endl;
    // initialize(prods);
    // getClosureSet(os);
    // fillReduceAction(os);
    return failure;
}
} // namespace LL
