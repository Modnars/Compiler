// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammerAnalysis.hpp"
#include "GrammerParser.hpp"

int main(int argc, const char *argv[]) {
    std::vector<std::string> rights;
    rights.push_back("1");
    rights.push_back("2");
    rights.push_back("3");
    Production p("left", rights);
    std::cout << p << std::endl;
    rights.push_back("4");
    std::cout << p << std::endl;
    read_grammer("hello");
    return EXIT_SUCCESS;
}
