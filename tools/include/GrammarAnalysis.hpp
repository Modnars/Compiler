// Name   : GrammarAnalysis.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMAR_ANALYSIS_HPP
#define __GRAMMAR_ANALYSIS_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

extern const std::string NONE;
extern const std::string BLUE;
extern const std::string GREEN;
extern const std::string RED;
extern const std::string WHITE;
extern const std::string YELLOW;

extern std::vector<std::string> split(const std::string &, const std::string &);
extern std::string &trim(std::string &);

class Production;
class Item;

extern std::ostream &operator<<(std::ostream &, const Production &);
extern std::ostream &operator<<(std::ostream &, const Item &);
extern bool operator<(const Item &a, const Item &b);
extern bool operator==(const Item &a, const Item &b);

/**
 * Check whether the `set` contains `value` as key.
 */
template <typename T>
extern bool contains(std::set<T> &set, const T &value) {
    if (set.empty())
        return false;
    return set.find(value) != set.end();
}

/**
 * Check whether the vector contains `value`.
 */
template <typename T>
extern bool contains(const std::vector<T> &vec, const T &val) {
    if (vec.empty())
        return false;
    for (auto var : vec)
        if (var == val)
            return true;
    return false;
}

/**
 * Check whether the `map` contains `value` as key.
 */
template <typename K, typename T>
extern bool contains(const std::map<K, T> &map, const K &val) {
    return map.find(val) != map.end();
}

/**
 * The `Production`, which is used to store the grammar under a formatible way.
 */
class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &rights) : left(left), rights(rights) {}

    const std::string left;
    const std::vector<std::string> rights;

    // Set the Production's expression for `null` and `start`.
    static void setNull(const std::string &);
    static std::string null();
    static void setStart(const std::string &);
    static std::string start();

private:
    static std::string _null;
    static std::string _start;
};

/**
 * The `Item`, which is used for store the status as the basic element in closure.
 */
class Item {
public:
    const std::string left;
    const std::vector<std::string> rights;
    const std::string search;

    Item(const std::string &left, const std::vector<std::string> &rights) : left(left), rights(rights), search("") {
        checkRep();
    }
    Item(const std::string &left, const std::vector<std::string> &rights, const std::string &search)
        : left(left), rights(rights), search(search) {
        checkRep();
    }

    bool could_reduce() const;
    bool is_reduced_by(const Production &) const;
    std::size_t pos() const;
    std::string nxsb() const; // Get the status (dot) next symbol.
    Item shift() const;

    // Set and get the class Item's status mark symbol, whose default value is "@".
    static void setMark(const std::string &);
    static std::string mark();

private:
    static std::string _mark;
    std::size_t _pos = 0; // Store the item's dot mark's position.
    void checkRep();
};

#endif /* __GRAMMAR_ANALYSIS_HPP */
