// Name   : main.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <fstream>
#include <vector>
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

int test_for_token_output() {
    set_enter_test_color("Token's output");
    Token tt = Token("if", Mark::IF);
    std::cout << tt << std::endl;
    std::cout << "OK!" << std::endl;
    set_exit_test_color();
    return EXIT_SUCCESS;
}

int test_for_token_value() {
    set_enter_test_color("Token's value");
    std::vector<std::shared_ptr<Token>> tokenVec;
    tokenVec.push_back(make_token(100));
    tokenVec.push_back(make_token(true));
    tokenVec.push_back(make_token('c'));
    tokenVec.push_back(make_token(123.123));
    // Warning: When you want to use the token to store a string's value, please 
    //          make sure you passed a string to it. Otherwise it might be 
    //          recognized as a boolean value.
    // For example: make_token("Hello"); 
    // The function will return a pointer to a token object whose value is a 'true' 
    // boolean value.
    tokenVec.push_back(make_token(std::string("Hello"))); 
    for (auto ptr : tokenVec) {
        std::cout << *ptr << std::endl;
    }
    set_exit_test_color();
    return EXIT_SUCCESS;
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

int test_for_scanner_scan() {
    set_enter_test_color("Scanner's scan function");
    Scanner scanner;
    std::ifstream is("../test/main.m");
    while (is) {
        std::cout << *scanner.scan(is) << std::endl;
    }
    set_exit_test_color();
    return EXIT_SUCCESS;
}

int main(int argc, const char *argv[]) {
    //test_for_token_output();
    //test_for_token_value();
    //test_for_scanner_constructor();
    test_for_scanner_scan();
    return EXIT_SUCCESS;
}
