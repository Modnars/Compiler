// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarParser.hpp"

void test_for_read_grammar() {
    for (auto pptr : read_grammar("../file/grammar.txt")) {
        std::cout << *pptr << std::endl;
    }
    std::cout << "START of Production: " << Production::start() << std::endl;
    std::cout << "NULL_MARK of Production: " << Production::null() << std::endl;
}

void test_for_SLR() {
    SLR::analyze(read_grammar("../file/grammar.txt"));
}

void test_for_LR() {
    LR::analyze(read_grammar("../file/grammar.txt"));
}

int main(int argc, const char *argv[]) {
    // test_for_read_grammar();
    test_for_LR();
    return EXIT_SUCCESS;
}
