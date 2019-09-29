// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

void test_for_read_grammar() {
    SLR::analysis(read_grammar("../file/grammar.txt"));
}

int main(int argc, const char *argv[]) {
    test_for_read_grammar();
    return EXIT_SUCCESS;
}
