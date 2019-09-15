// Name   : SymbolTable.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>
#include <string>

#include "Base.hpp"

class Symbol {
public:
    Symbol() : type(VOID) { }

private:
    enum { VOID, INT, BOOL, REAL, STRING } type;
    int decl_pos;
    int offset;
    int width;
    int params;
    bool is_global;
};

class SymbolTable {
public:
    SymbolTable(const std::string &s) : name(s) { }

private:
    std::string name;
    std::map<std::string, std::shared_ptr<Symbol>> content;
};

#endif /* SYMBOL_TABLE_HPP */
