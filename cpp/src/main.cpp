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
const char *RED     = "\e[0;31m";
const char *WHITE   = "\e[1;37m";
const char *YELLOW  = "\e[1;33m";

int test() {
    printf("%s", RED); // TODO Colorful output.
    std::cout << ">>>>>>>>>> Enter Test <<<<<<<<<<" << std::endl;
    printf("%s", YELLOW); // TODO Colorful output.
    Scanner sc;
    for (auto iter = sc.words->begin(); iter != sc.words->end(); ++iter) {
        std::cout << *(iter->second) << std::endl;
    }
    printf("%s", RED); // TODO Colorful output.
    std::cout << ">>>>>>>>>>   RETURN   <<<<<<<<<<" << std::endl;
    printf("%s", NONE); // TODO Colorful output.
    return EXIT_SUCCESS;
}

int main(int argc, const char *argv[]) {
    scan(std::cerr);
    Token tt = Token("if", Mark::IF);
    std::cout << tt << std::endl;
    std::cout << "OK!" << std::endl;
    test();
    return EXIT_SUCCESS;
}
