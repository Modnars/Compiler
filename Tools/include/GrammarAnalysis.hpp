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

#endif /* __GRAMMAR_ANALYSIS_HPP */
