// Name   : GrammarAnalysis.cpp
// Author : Mondar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <cstdlib>

#include "GrammarAnalysis.hpp"

const std::string NONE    = "\e[0m";
const std::string BLUE    = "\e[0;34m";
const std::string GREEN   = "\e[0;32m";
const std::string RED     = "\e[0;31m";
const std::string WHITE   = "\e[1;37m";
const std::string YELLOW  = "\e[1;33m";

/**
 * Split the source string `str` at `delim`.
 */
std::vector<std::string> split(const std::string &str, const std::string &delim) { 
    std::vector<std::string> res;
    if ("" == str) return res;
    std::string strs = str + delim; 
    int pos, size = strs.size();
    for (int i = 0; i < size; ++i) {
        pos = strs.find(delim, i); 
        if (pos < size) { 
            res.emplace_back(strs.substr(i, pos-i)); 
            i = pos + delim.size() - 1;
        }
    }
    return res;	
}

std::string &trim(std::string &s) {
    if (s.empty()) return s;
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

std::ostream &operator<<(std::ostream &os, const Production &p) {
    os << p.left << " -> ";
    for (auto right : p.rights) {
        os << right << " ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Item &it) {
    os << it.left << " -> ";
    for (auto var : it.rights)
        os << var << " ";
    if (it.search != "")
        os << "Search: " << it.search;
    return os;
}

bool operator<(const Item &a, const Item &b) {
    if (a.left < b.left)
        return true;
    else if (a.left == b.left && a.rights < b.rights)
        return true;
    else if (a.left == b.left && a.rights == b.rights && a.search < b.search)
        return true;
    return false;
}

bool operator==(const Item &a, const Item &b) {
    return (a.left == b.left) && (a.rights == b.rights) && (a.search == b.search);
}

/* Class Production's implementation. */

/**
 * Set the Production's null-production contents.
 * @param null The null production's expression.
 */
void Production::setNull(const std::string &null) {
    _null = null;
}

/**
 * Get the Production's null production contents.
 * @return the null production's expression.
 */
std::string Production::null() {
    return _null;
}

/* Initialize the Production's null production contents with "$" */
std::string Production::_null = "$";

/**
 * Set the Production's start-production contents.
 * @param start The start production's expression.
 */
void Production::setStart(const std::string &start) {
    _start = start;
}

/**
 * Get the Production's start production contents.
 * @return the start production's expression.
 */
std::string Production::start() {
    return _start;
}

/* Initialize the Production's start production contents with "S" */
std::string Production::_start = "S";
/* Class Item's implementation. */

/**
 * Judge wheter the Item could reduce from a Production.
 * @return true if the Item could be reduced by a production, otherwise false.
 */
bool Item::could_reduce() const {
    return (rights.back() == _mark || rights.back() == Production::null());
}

/**
 * Judge wheter the Item could be reduced by the parameter.
 * @param The production object need to be judged.
 * @return true if the Item could be reduced by the parameter, otherwise false.
 */
bool Item::is_reduced_by(const Production &p) const {
    if (!could_reduce()) return false;
    if (left != p.left) return false;
    if (rights.back() == Production::null() && p.rights[0] == Production::null())
        return true;
    for (std::size_t i = 0; i < p.rights.size(); ++i) 
        if (rights[i] != p.rights[i]) return false;
    return true;
}

/**
 * Get the Item's status dot's position.
 * @return the dot's index.
 */
// Warning: the function is different from HIT-Compiler-Experiment. If could not 
//          find the mark, return rights.size();
std::size_t Item::pos() const {
    return _pos;
}

/**
 * Get the status (dot) next symbol.
 * @return the symbol whose index is pos+1.
 */
std::string Item::nxsb() const { 
    if (could_reduce()) return "";
    return rights[_pos+1];
}

/**
 * Get the item's next item after a shift operation.
 * @return the next item of current item.
 */
Item Item::shift() const {
    auto tmp = rights;
    std::swap(tmp[_pos], tmp[_pos+1]);
    return Item(left, tmp, search);
}

/**
 * Set the Item's status dot contents.
 * @param mark The status dot's expression.
 */
void Item::setMark(const std::string &mark) {
    _mark = mark;
}

/**
 * Get the Item's status dot contents.
 * @return the status dot's expression.
 */
std::string Item::mark() {
    return _mark;
}

/* Initialize the Item's dot contents with "@" */
std::string Item::_mark = "@";

/**
 * Check whether the grammar is right with the configure of `Item::_mark`.
 */
void Item::checkRep() {
    std::size_t i;
    for (i = 0; i < rights.size(); ++i) 
        if (rights[i] == _mark) break;
    if (i == rights.size()) {
        std::cerr << RED << "Failed to construct Item object." << NONE << std::endl;
        exit(EXIT_FAILURE);
    }
    _pos = i;
}
