// Name   : GrammarAnalysis.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_ANALYSIS_HPP
#define __GRAMMAR_ANALYSIS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <set>

extern const std::string NONE;
extern const std::string BLUE;
extern const std::string GREEN;
extern const std::string RED;
extern const std::string WHITE;
extern const std::string YELLOW;

class Production;
class Item;

extern std::vector<std::string> split(const std::string &, const std::string &);
extern std::string &trim(std::string &);

template <typename T>
extern bool contains(std::set<T> &set, const T &value) {
    if (set.empty())
        return false;
    return set.find(value) != set.end();
}

extern std::ostream &operator<<(std::ostream &, const Production &);

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &rights) :
        left(left), rights(rights) { }

    const std::string left;
    const std::vector<std::string> rights;

    // Set the Production's empty string contents.
    static void setNull(const std::string&);
    static std::string null();

private:
    static std::string _null;
};

class Item {
public:
    const std::string left;
    const std::vector<std::string> rights;
    const std::string search;

    Item(const std::string &left, const std::vector<std::string> &rights) :
        left(left), rights(rights), search("") { }
    Item(const std::string &left, const std::vector<std::string> &rights, 
            const std::string &search) :
        left(left), rights(rights), search(search) { }

    bool could_reduce() const ;
    bool is_reduced_by(const Production &) const ;
    std::size_t pos() const ;
    std::string nxsb() const ; // Get the status (dot) next symbol.
    Item shift() const ;

    // Set and get the class Item's status mark symbol, whose default value is "@".
    static void setMark(const std::string &);
    static std::string mark();

private:
    static std::string _mark;
};

#endif /* __GRAMMAR_ANALYSIS_HPP */
