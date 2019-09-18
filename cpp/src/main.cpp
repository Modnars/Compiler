// Name   : main.hpp
// Author : Modnar
// Date   : 2019-05-18
// Copyright (C) 2019 Modnar. All rights reserved.

#include <iostream>
#include <cstdlib>

#include "scanner.hpp"

int main(int argc, const char *argv[]) {
    scan(std::cerr);
    Token tt(Mark::INT);
    std::cout << tt << std::endl;
    std::cout << "OK!" << std::endl;
    return EXIT_SUCCESS;
}
