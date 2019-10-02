// Name   : Test.cpp
// Author : Modnar
// Date   : 2019-09-24
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

namespace Test {
    void set_enter_test_color(std::string title = "") {
        if (title != "") {
            std::cout << GREEN << "-> Test for " << title << ":" << std::endl;
        }
        std::cout << RED << ">>>>>>>>>> ENTER TEST <<<<<<<<<<" << YELLOW
                  << std::endl;
    }

    void set_exit_test_color() {
        std::cout << RED << ">>>>>>>>>>   RETURN   <<<<<<<<<<\n" << NONE 
                  << std::endl;
    }
    
    void test_for_parser() {
        set_enter_test_color("Parser");
        printf("Test\n");
        set_exit_test_color();
    }

    void test_for_SLR() {
        set_enter_test_color("SLR");
        std::string f = "/Users/StevenShen/Project/CompExp/file/grammar/grammar1.txt";
        SLR::analyze(read_grammar(f));
        set_exit_test_color();
    }

    void test_for_LR() {
    }

    void test_all() {
        // test_for_parser();
        test_for_SLR();
    }
}
