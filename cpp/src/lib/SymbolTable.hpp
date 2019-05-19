// Name   : SymbolTable.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>

class Symbol {
public:
private:
    enum { INT, VOID, BOOL, REAL, STRING } type;
};

class SymbolTable {
public:
    SymbolTable() { }

private:
    std::string name;
    std::map<std::string, Symbol> content;
};

#endif /* SYMBOL_TABLE_HPP */
