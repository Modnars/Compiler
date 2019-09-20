// Name   : main.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include <cstdio> // For printing colorful text.

#include "scanner.hpp"

const char *NONE    = "\e[0m";
const char *BLUE    = "\e[0;34m";
const char *GREEN   = "\e[0;32m";
const char *RED     = "\e[0;31m";
const char *WHITE   = "\e[1;37m";
const char *YELLOW  = "\e[1;33m";

void set_enter_test_color(std::string title = "") {
    if (title != "") {
        printf("%s", GREEN); 
        std::cout << "-> Test for " << title << ":" << std::endl;
    }
    printf("%s", RED); 
    std::cout << ">>>>>>>>>> ENTER TEST <<<<<<<<<<" << std::endl;
    printf("%s", YELLOW); 
}

void set_exit_test_color() {
    printf("%s", RED); 
    std::cout << ">>>>>>>>>>   RETURN   <<<<<<<<<<\n" << std::endl;
    printf("%s", NONE); 
}

int test_for_scanner_constructor() {
    set_enter_test_color("Scanner's constructor");
    Scanner sc;
    for (auto iter = sc.words->begin(); iter != sc.words->end(); ++iter) {
        std::cout << *(iter->second) << std::endl;
    }
    set_exit_test_color();
    return EXIT_SUCCESS;
}

int test_for_token_output() {
    set_enter_test_color("Token's output");
    Token tt = Token("if", Mark::IF);
    std::cout << tt << std::endl;
    std::cout << "OK!" << std::endl;
    set_exit_test_color();
    return EXIT_SUCCESS;
}

int main(int argc, const char *argv[]) {
    test_for_token_output();
    test_for_scanner_constructor();
    return EXIT_SUCCESS;
}
