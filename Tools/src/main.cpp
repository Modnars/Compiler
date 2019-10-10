// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarParser.hpp"

void error_with_details(const std::string &msg) {
    std::cerr << RED << msg << NONE << std::endl;
}

int main(int argc, const char *argv[]) {
    std::string prog = argv[0], null = "$", option;
    int ret = EXIT_SUCCESS;
    if (argc == 3 || argc == 4) {
        option = argv[1];
        if (argc == 4)
            null = trim(split(argv[3], "=")[1]);
        if (option == "-slr") {
            if (argc == 3)
                SLR::analyze(read_grammar(argv[2]));
            else
                SLR::analyze(read_grammar(argv[2], null));
        } else if (option == "-lr") {
            if (argc == 3)
                LR::analyze(read_grammar(argv[2]));
            else
                LR::analyze(read_grammar(argv[2], null));
        } else if (option == "-all") {
            if (argc == 3) {
                SLR::analyze(read_grammar(argv[2]));
                std::cout << std::endl;
                LR::analyze(read_grammar(argv[2]));
            } else {
                SLR::analyze(read_grammar(argv[2], null));
                std::cout << std::endl;
                LR::analyze(read_grammar(argv[2], null));
            }
        } else {
            error_with_details("Please check the option, which must be -slr or -lr!");
            ret = EXIT_FAILURE;
        }
    } else {
        error_with_details("Usage : "+prog+" -[option] filename [-nil='null']");
        ret = EXIT_FAILURE;
    }        
    return ret;
}
