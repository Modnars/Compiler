// Name   : main.cpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "GrammarParser.hpp"

bool Parser::SHOW_DETAILS = true;

std::string usage(const std::string &prog) {
    return "Usage : " + prog + " -[option argv] [-i]\n" +
           "\t-a filepath: Use all the methods to analyze grammar file.\n" +
           "\t-l filepath: Use `LL(1)` method to analyze grammar file.\n" +
           "\t-s filepath: Use `SLR(1)` method to analyze grammar file.\n" +
           "\t-r filepath: Use `LR(1)` method to analyze grammar file.\n" +
           "\t-n symbol: Use `symbol` as the null symbol of grammar file.\n" + "\t-i: Ignore the execution details.";
}

void error_with_details(const std::string &msg) {
    std::cerr << RED << msg << NONE << std::endl;
}

void feedback(bool failure, const std::string &method) {
    if (failure) {
        std::cerr << RED << "[FAILED] The grammar is not the `" << method << "` grammar." << std::endl;
    } else {
        std::cerr << GREEN << "[SUCCESS] The grammar is the `" << method << "` grammar." << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::string prog = argv[0], null = "$", filepath;
    int opt = 0, func = -1;
    while ((opt = getopt(argc, argv, "a:l:s:r:n:i")) != -1) {
        switch (opt) {
            case 'a':
                func     = 0;
                filepath = optarg;
                break;
            case 'l':
                func     = 1; // LL(1)
                filepath = optarg;
                break;
            case 's':
                func     = 2; // SLR(1)
                filepath = optarg;
                break;
            case 'r':
                func     = 3; // LR(1)
                filepath = optarg;
                break;
            case 'n':
                null = optarg;
                break;
            case 'i':
                Parser::SHOW_DETAILS = false;
                break;
            default:
                error_with_details(usage(prog));
                exit(EXIT_FAILURE);
        }
    }
    switch (func) {
        case 0:
            feedback(LL::analyze(read_grammar(filepath, null)), "LL(1)");
            feedback(SLR::analyze(read_grammar(filepath, null)), "SLR(1)");
            feedback(LR::analyze(read_grammar(filepath, null)), "LR(1)");
            break;
        case 1:
            feedback(LL::analyze(read_grammar(filepath, null)), "LL(1)");
            break;
        case 2:
            feedback(SLR::analyze(read_grammar(filepath, null)), "SLR(1)");
            break;
        case 3:
            feedback(LR::analyze(read_grammar(filepath, null)), "LR(1)");
            break;
        default:
            error_with_details("Please choose the correct option and offer the right filepath.");
    }
    return EXIT_SUCCESS;
}
