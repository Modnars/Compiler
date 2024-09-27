// Name   : LR.cpp
// Author : Modnar
// Date   : 2019-09-27
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdint>
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
// Store the Closures' set.
std::vector<std::set<Item>> ClosureSet;

// Store the action information of REDUCE, SHIFT and GOTO.
std::map<std::size_t, std::shared_ptr<std::map<std::string, std::size_t>>> ActionTable;

// Store the State Stack information.
std::stack<int> StateStack;
// Store the Symbol Stack information.
std::stack<std::string> SymbolStack;

bool failure;
} // namespace

namespace LR {
// Judge whether the symbol could generate null symbol.
// For example, S -> E, E -> $, we see the S could generate '$'.
bool could_be_null(const std::string &sym) {
    return contains(*FirstSet[sym], Production::null());
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
    bool extending = true;
    while (extending) {
        extending = false;
        for (auto pptr : ProdVec) {
            auto left     = pptr->left;
            bool add_null = false;
            for (size_t i = 0; i < pptr->rights.size(); ++i) {
                if (could_be_null(pptr->rights[i]) && i == pptr->rights.size() - 1) {
                    add_null = true;
                } else if (!could_be_null(pptr->rights[i])) {
                    for (auto var : *FirstSet[pptr->rights[i]]) {
                        if (!contains(*FirstSet[left], var)) {
                            FirstSet[left]->push_back(var);
                            extending = true;
                        }
                    }
                    break;
                } else if (could_be_null(pptr->rights[i])) {
                    for (auto var : *FirstSet[pptr->rights[i]]) {
                        if (!contains(*FirstSet[left], var) && var != Production::null()) {
                            FirstSet[left]->push_back(var);
                            extending = true;
                        }
                    }
                }
            }
            if (add_null && !contains(*FirstSet[left], Production::null()))
                FirstSet[left]->push_back(Production::null());
        }
    }
    // Remove Production::null() from TerminalSet
    for (auto iter = TerminalSet.begin(); iter != TerminalSet.end(); ++iter)
        if (*iter == Production::null()) {
            TerminalSet.erase(iter);
            break;
        }
    // Remove Production::null() from FirstSet
    for (auto iter = FirstSet.begin(); iter != FirstSet.end(); ++iter)
        if (iter->first == Production::null()) {
            FirstSet.erase(iter);
            break;
        }
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
    std::cout << "FirstSet: " << FirstSet.size() << std::endl;
    for (const auto &kv : FirstSet)
    {
        std::cout << kv.first << ": ";
        for (const auto &item: *kv.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}

// Extend the closure when the closure needs to extending.
// For example, we can use the function to get status I0 after we put start item
// in closure.
void extend(std::set<Item> &closure) {
    std::vector<std::string> rights;
    std::string left;
    bool extending = true;
    while (extending) {
        extending = false;
        for (auto iter = closure.begin(); iter != closure.end(); ++iter) {
            for (auto pptr : ProdVec) {
                if ((left = pptr->left) == iter->nxsb()) {
                    rights = {Item::mark()};
                    for (auto var : pptr->rights)
                        rights.push_back(var);
                    // The item's search is from extending.
                    auto new_item = Item(left, rights, iter->search);
                    int pos       = iter->pos() + 1;
                    bool need_add = true;
                    while (pos != iter->rights.size() - 1) {
                        ++pos;
                        for (auto sym : *FirstSet[iter->rights[pos]]) {
                            auto add_new_item = Item(left, rights, sym);
                            if (!contains(closure, add_new_item) && sym != Production::null()) {
                                closure.insert(add_new_item);
                                extending = true;
                            }
                        }
                        if (!could_be_null(iter->rights[pos])) {
                            need_add = false;
                            break;
                        }
                    } /* while (pos != iter->rights.size()-1) */
                    if (need_add && !contains(closure, new_item)) {
                        closure.insert(new_item);
                        extending = true;
                    }
                } /* if ((left = pptr->left) ...) */
            }     /* for (auto pptr : ...) loop */
        }         /* for (auto iter ...) loop */
    }             /* while (extending) */
}

// Get the first closure.
void getI0() {
    std::vector<std::string> rights = {Item::mark()};
    for (auto var : ProdVec[0]->rights)
        rights.push_back(var);
    std::set<Item> closure = {Item(ProdVec[0]->left, rights, "#")}; // Put the start item in closure.
    extend(closure);
    ClosureSet.push_back(closure);
}

// Get all the closures.
void getClosureSet(std::ostream &os = std::cout) {
    getI0();
    std::set<Item> closure;
    std::vector<std::string> tmpVec, rights; // Store the temp next symbol.
    for (std::size_t i = 0UL; i < ClosureSet.size(); ++i) {
        tmpVec.clear();
        for (auto item : ClosureSet[i]) {
            if (!item.could_reduce() && !contains(tmpVec, item.nxsb()))
                tmpVec.push_back(item.nxsb());
        }
        for (auto sym : tmpVec) {
            closure.clear();
            for (auto item : ClosureSet[i]) {
                if (!item.could_reduce() && item.nxsb() == sym) {
                    closure.insert(item.shift());
                }
            }
            extend(closure);
            bool found = false; // Mark whether find the closure from before closures.
            for (std::size_t j = 0UL; j < ClosureSet.size(); ++j) {
                if (ClosureSet[j] == closure) {
                    // Use new data structure. Here...
                    if (!contains(ActionTable, i)) {
                        ActionTable[i]         = std::make_shared<std::map<std::string, std::size_t>>();
                        (*ActionTable[i])[sym] = j;
                    } else {
                        if (!contains(*ActionTable[i], sym)) {
                            (*ActionTable[i])[sym] = j;
                        } else {
                            for (auto item : closure)
                                os << item << std::endl;
                            if (Parser::SHOW_DETAILS)
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
                    ActionTable[i]         = std::make_shared<std::map<std::string, std::size_t>>();
                    (*ActionTable[i])[sym] = ClosureSet.size() - 1UL;
                } else {
                    if (!contains(*ActionTable[i], sym)) {
                        (*ActionTable[i])[sym] = ClosureSet.size() - 1;
                    } else {
                        if (Parser::SHOW_DETAILS)
                            os << "Error! [2] The Gramma fill the ActionTable repeatly!" << std::endl;
                        failure = true;
                    }
                } // ... Done
            }
        }
    }
    os << ClosureSet.size() << std::endl;
    for (const auto &closure: ClosureSet)
    {
        for (const auto &item: closure)
            os << item << std::endl;
        os << std::endl;
    }
}

// Fill the ReduceTable when has gotten all the closures.
// This function is also the key to judge whether the grammar is LR(1) grammar.
void fillReduceAction(std::ostream &os = std::cerr) {
    int base = ClosureSet.size();
    for (std::size_t i = 0UL; i < ClosureSet.size(); ++i) {
        for (auto item : ClosureSet[i]) {
            if (item.could_reduce()) {
                for (int j = 0; j < ProdVec.size(); ++j) {
                    if (item.is_reduced_by(*ProdVec[j])) {
                        if (!contains(ActionTable, i)) {
                            ActionTable[i]                 = std::make_shared<std::map<std::string, std::size_t>>();
                            (*ActionTable[i])[item.search] = base + j;
                        } else {
                            if (!contains(*ActionTable[i], item.search)) {
                                (*ActionTable[i])[item.search] = base + j;
                            } else if ((*ActionTable[i])[item.search] != base + j) {
                                if (Parser::SHOW_DETAILS) {
                                    os << "\nError! [3] The Gramma fill the ActionTable repeatly!"
                                       << "\nProduction: " << *ProdVec[j] << "\nPos: " << i << ", " << item.search
                                       << "\nHave Existed: " << (*ActionTable[i])[item.search] << std::endl;
                                }
                                failure = true;
                            }
                        }
                        break;
                    }
                } /* for (size_t j = 0; ...) loop */
            }     /* if (item.could_reduce()) */
        }         /* for (auto item : ...) loop */
    }             /* for (size_t i = 0; ...) loop */
}

bool analyze(const std::vector<std::shared_ptr<Production>> &prods, std::ostream &os) {
    failure = false;
    initialize(prods);
    getClosureSet(os);
    fillReduceAction(os);
    return failure;
}
} // namespace LR
