// Name   : Test.cpp
// Author : Modnar
// Date   : 2019-09-24
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "GrammarAnalysis.hpp"
#include "GrammarParser.hpp"

namespace Test {
    void enter_test(std::string title = "") {
        if (title != "") {
            std::cout << GREEN << "-> Test for " << title << ":" << std::endl;
        }
        std::cout << RED << ">>>>>>>>>> ENTER TEST <<<<<<<<<<" << YELLOW
                  << std::endl;
    }

    void exit_test() {
        std::cout << RED << ">>>>>>>>>>   RETURN   <<<<<<<<<<\n" << NONE 
                  << std::endl;
    }

    void test_for_read_grammar() {
        enter_test("Read_Grammar");
        std::string f = "../res/txt/grammar.txt";
        for (auto pptr : read_grammar(f))
            std::cout << *pptr << std::endl;
        exit_test();
    }

    void test_for_SLR() {
        enter_test("SLR(1)");
        std::string f = "../res/txt/grammar.txt";
        std::cout << ((SLR::analyze(read_grammar(f)) == true) ? 
            "TEST SUCCESS" : "TEST FAILED") << std::endl;
        exit_test();
    }

    void test_for_LR() {
        enter_test("SLR(1)");
        std::string f = "../res/txt/grammar.txt";
        std::cout << ((LR::analyze(read_grammar(f)) == false) ? 
            "TEST SUCCESS" : "TEST FAILED") << std::endl;
        exit_test();
    }

    void test_all() {
        test_for_read_grammar();
        test_for_SLR();
        test_for_LR();
    }
}
