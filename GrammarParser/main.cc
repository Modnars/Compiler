/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:03:11
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <iostream>

#include "lr1.h"
#include "util.h"

int main(int argc, char *argv[]) {
    Grammar grammar;
    int ret = grammar.ReadFromFile(argv[1]);
    if (ret != 0) {
        util::LOG_ERROR("Failed to read grammar.");
    }
    // grammar.CalcFirstSet();
    // slr::Parser parser{grammar};
    // parser.Parse();
    // parser.ShowDetails();
    mcc::LR1Parser parser{grammar};
    parser.Parse();
    parser.ShowDetails();
    parser.Analyze(std::cin);
    return 0;
}
