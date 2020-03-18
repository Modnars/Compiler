// Name   : SLR.cpp
// Author : Modnar
// Date   : 2019-09-27
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <map>

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
    // Store the Action information of REDUCE, SHIFT and GOTO.
    std::map<int, std::shared_ptr<std::map<std::string, int>>> ActionTable;
    
    std::stack<int> StateStack;  // Store the State Stack information.
    std::stack<std::string> SymbolStack; // Store the Symbol Stack information.

    // Mark the analysis result.
    bool failure;
}

namespace SLR {
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
        while (extending) {
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
        }
    }

    // Get the FOLLOW Set of the grammar.
    void getFollowSet() {
        for (auto var : NonTerminalSet) 
            FollowSet[var] = std::make_shared<std::vector<std::string>>();
        FollowSet[Production::start()]->push_back("#");
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

    void extend(std::vector<Item> &closure) {
        std::vector<std::string> rights;
        std::string left;
        for (size_t i = 0; i < closure.size(); ++i) {
            for (auto pptr : ProdVec) {
                if ((left = pptr->left) == closure[i].nxsb()) {
                    rights = {Item::mark()};
                    for (auto var : pptr->rights) 
                        rights.push_back(var);
                    auto new_item = Item(left, rights);
                    if (!contains(closure, new_item))
                        closure.push_back(new_item);
                }
            } 
        }
    }

    void getI0() {
        std::vector<std::string> rights = {Item::mark()};
        for (auto var : ProdVec[0]->rights)
            rights.push_back(var);
        std::vector<Item> closure = {Item(ProdVec[0]->left, rights)};
        std::string left;
        for (size_t i = 0; i < closure.size(); ++i) {
            for (auto pptr : ProdVec) {
                if ((left = pptr->left) == closure[i].nxsb()) {
                    rights = {Item::mark()};
                    for (auto var : pptr->rights) 
                        rights.push_back(var);
                    auto new_item = Item(left, rights);
                    if (!contains(closure, new_item))
                        closure.push_back(new_item);
                }
            } 
        }
        ClosureSet.push_back(closure);
    }

    // Get all the closures.
    void getClosureSet(std::ostream &os = std::cout) {
        getI0();
        std::vector<Item> closure;
        std::vector<std::string> tmpVec, rights; // Store the temp next symbol.
        for (int i = 0; i < ClosureSet.size(); ++i) {
            tmpVec.clear();
            for (auto item : ClosureSet[i]) {
                if (!item.could_reduce() && !contains(tmpVec, item.nxsb()))
                    tmpVec.push_back(item.nxsb());
            }
            for (auto sym : tmpVec) {
                closure.clear();
                for (auto item : ClosureSet[i]) {
                    if (!item.could_reduce() && item.nxsb() == sym) {
                        closure.push_back(item.shift());
                    }
                }
                extend(closure);
                bool found = false; // Mark whether find the closure from before closures.
                for (int j = 0; j < ClosureSet.size(); ++j) {
                    if (ClosureSet[j] == closure) {
                        // Use new data structure. Here...
                        if (!contains(ActionTable, i)) {
                            ActionTable[i] = std::make_shared<std::map<std::string, int>>();
                            (*ActionTable[i])[sym] = j;
                        } else {
                            if (!contains(*ActionTable[i], sym)) {
                                (*ActionTable[i])[sym] = j;
                            } else {
                                for (auto item : closure) 
                                    os << item << std::endl;
                                os << "Error! [1] The Gramma fill the ActionTable repeatly!" << std::endl;
                                failure = true;
                            }
                        } // ... Done
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ClosureSet.push_back(closure);
                    // Use new data structure. Here...
                    if (!contains(ActionTable, i)) {
                        ActionTable[i] = std::make_shared<std::map<std::string, int>>();
                        (*ActionTable[i])[sym] = ClosureSet.size()-1;
                    } else {
                        if (!contains(*ActionTable[i], sym)) {
                            (*ActionTable[i])[sym] = ClosureSet.size()-1;
                        } else {
                            os << "Error! [2] The Gramma fill the ActionTable repeatly!" << std::endl;
                            failure = true;
                        }
                    } // ... Done
                }
            }
        }
    }

    // Fill the ReduceTable when has gotten all the closures.
    // This function is also the key to judge whether the grammar is SLR(1) grammar.
    void fillReduceAction(std::ostream &os = std::cout) {
        int base = ClosureSet.size();
        for (int i = 0; i < ClosureSet.size(); ++i) {
            for (auto item : ClosureSet[i]) {
                if (item.could_reduce()) {
                    for (int j = 0; j < ProdVec.size(); ++j) {
                        if (item.is_reduced_by(*ProdVec[j])) {
                            for (auto sym : *FollowSet[item.left]) {
                                if (!contains(ActionTable, i)) {
                                    ActionTable[i] = std::make_shared<std::map<std::string, int>>();
                                    (*ActionTable[i])[sym] = base + j;
                                } else {
                                    if (!contains(*ActionTable[i], sym)) {
                                        (*ActionTable[i])[sym] = base + j;
                                    } else if ((*ActionTable[i])[sym] != base+j) {
                                        os << "\nError! [3] The Gramma fill the ActionTable repeatly!" << std::endl;
                                        os << "Production: " << *ProdVec[j] << std::endl;
                                        os << "Pos: " << i << ", " << item.search << std::endl;
                                        os << "Have Existed: " << (*ActionTable[i])[item.search] << std::endl;
                                        failure = true;
                                    }
                                        
                                }
                            }
                            break;
                        }
                    } /* for (size_t j = 0; ...) loop */
                } /* if (item.could_reduce()) */
            } /* for (auto item : ...) loop */
        } /* for (size_t i = 0; ...) loop */
    }

    bool analyze(const std::vector<std::shared_ptr<Production>> &prods, 
            std::ostream &os) {
        failure = false;
        initialize(prods);
        getClosureSet(os);
        fillReduceAction(os);
        return failure;
    }
}
