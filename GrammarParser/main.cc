/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:03:11
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <iostream>

#include "parser.h"
#include "slr.h"

int main(int argc, char *argv[]) {
    Grammar grammar;
    int ret = ReadGrammar(argv[1], grammar);
    if (ret != 0) {
        std::cerr << Color::RED << "Failed to read grammar." << Color::RESET << std::endl;
    }
    slr::Parser parser{grammar};
    parser.Parse();
    parser.ShowDetails();
    return 0;
}
