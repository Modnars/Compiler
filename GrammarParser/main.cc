/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:03:11
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <cstring>
#include <fstream>
#include <iostream>

#include "lr1.h"
#include "slr.h"
#include "util.h"

void usage() {
    std::cout << "usage: GrammarParser filepath" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        exit(1);
    }
    Grammar grammar;
    int ret = grammar.ReadFromFile(argv[1]);
    if (ret != 0) {
        util::LOG_ERROR("[ERROR] Failed to read grammar.");
        exit(1);
    }
    // mcc::LR1Parser parser{grammar};
    mcc::SLRParser parser{grammar};
    parser.Parse();
    parser.ShowDetails();
    auto filename = std::string(::basename(argv[1]));

    auto dotFile = std::ofstream(filename + ".dot");
    parser.OutputToGraphviz(dotFile);
    dotFile.close();

    auto csvFile = std::ofstream(filename + ".csv");
    parser.OutputToCsv(csvFile);
    csvFile.close();

    parser.Analyze(std::cin);
    return 0;
}
