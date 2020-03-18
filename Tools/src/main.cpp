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

void feedback(bool failure, const std::string &method) {
    if (failure) {
        std::cerr << RED << "[FAILED] The grammar is not the `" << method 
                  << "` grammar." << std::endl;
    } else {
        std::cerr << GREEN << "[SUCCESS] The grammar is the `" << method 
                  << "` grammar." << std::endl;
    }
}

int main(int argc, const char *argv[]) {
    std::string prog = argv[0], null = "$", option;
    int ret = EXIT_SUCCESS;
    if (argc == 3 || argc == 4) {
        option = argv[1]; // Get the user's target option.
        if (argc == 4) // The user set the `null` symbol in his grammar.
            null = trim(split(argv[3], "=")[1]);
        if (option == "-slr") {
            if (argc == 3)
                feedback(SLR::analyze(read_grammar(argv[2])), "SLR(1)");
            else
                feedback(SLR::analyze(read_grammar(argv[2], null)), "SLR(1)");
        } else if (option == "-lr") {
            if (argc == 3)
                feedback(LR::analyze(read_grammar(argv[2])), "LR(1)");
            else
                feedback(LR::analyze(read_grammar(argv[2], null)), "LR(1)");
        } else if (option == "-all") {
            if (argc == 3) {
                feedback(SLR::analyze(read_grammar(argv[2])), "SLR(1)");
                std::cerr << std::endl;
                feedback(LR::analyze(read_grammar(argv[2])), "LR(1)");
            } else {
                feedback(SLR::analyze(read_grammar(argv[2], null)), "SLR(1)");
                std::cerr << std::endl;
                feedback(LR::analyze(read_grammar(argv[2], null)), "LR(1)");
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
