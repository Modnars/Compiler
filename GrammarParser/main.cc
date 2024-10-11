/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:03:11
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <iostream>

#include "parser.h"
#include "slr.h"
#include "lr.h"

int main(int argc, char *argv[]) {
    Grammar grammar;
    int ret = grammar.ReadFromFile(argv[1]);
    if (ret != 0) {
        std::cerr << Color::RED << "Failed to read grammar." << Color::RESET << std::endl;
    }
    // grammar.CalcFirstSet();
    // slr::Parser parser{grammar};
    // parser.Parse();
    // parser.ShowDetails();
    lr::Parser parser{grammar};
    parser.Parse();
    parser.ShowDetails();
    return 0;
}
