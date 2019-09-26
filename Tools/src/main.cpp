// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

int main(int argc, const char *argv[]) {
    std::vector<std::string> rights;
    rights.push_back("1");
    rights.push_back("2");
    rights.push_back("3");
    Production p("left", rights);
    std::cout << p << std::endl;
    rights.push_back("4");
    std::cout << p << std::endl;
    read_grammar("hello");
    rights.push_back("@");
    Item item("left", rights);
    return EXIT_SUCCESS;
}
