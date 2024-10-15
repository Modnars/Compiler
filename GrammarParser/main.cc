/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:03:11
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <getopt.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

#include "grammar.h"
#include "ll1.h"
#include "lr1.h"
#include "slr.h"
#include "util.h"

void version() {
    std::cout << "GrammarParser v1.0.0\n"
              << "  Source: https://github.com/Modnars/Compiler\n"
              << "  Date: 2024.10.15\n"
              << std::flush;
}

void usage() {
    std::cout << "Usage: GrammarParser --file GRAMMAR_FILE [OPTION]\n"
              << "  -f, --file     Specify the source grammar file.\n"
              << "  -h, --help     Show the text and exit.\n"
              << "  -i, --input    Redirect the input stream with input file. If not set, the input stream is stdin.\n"
              << "  -o, --output   Output the ActionTable (in LR) and the PredictionTable (in LL) to file.\n"
              << "                 Output file types: Graphviz(.dot, `-o dot`), CSV(.csv, `-o csv`).\n"
              << "  -v, --version  Show the version information and exit.\n"
              << "  -V, --verbose  Show the version information and exit.\n"
              << "  --ll1          Parse the Grammar File with LL(1).\n"
              << "  --slr          Parse the Grammar File with SLR(1).\n"
              << "  --lr1          Parse the Grammar File with LR(1).\n"
              << std::flush;
}

int main(int argc, char *argv[]) {
    std::string grammarFilepath = "";
    std::string inputFilepath = "";
    std::string grammarType = "";
    std::shared_ptr<mcc::Parser> parser = nullptr;
    Grammar grammar;
    bool outDot, outCsv = false;

    int opt = 0;
    int option_index = 0;
    ::option long_options[] = {{"help", no_argument, 0, 'h'},       {"version", no_argument, 0, 'v'},
                               {"file", required_argument, 0, 'f'}, {"input", optional_argument, 0, 'i'},
                               {"ll1", no_argument, 0, 1},          {"slr", no_argument, 0, 2},
                               {"lr1", no_argument, 0, 3},          {"output", no_argument, 0, 'o'}};
    while ((opt = getopt_long(argc, argv, "hvf:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                usage();
                exit(0);
            case 'v':
                version();
                exit(0);
            case 'V':
                util::IsVerboseMode = true;
                break;
            case 'f':
                grammarFilepath = optarg;
                break;
            case 'i':
                inputFilepath = optarg;
                break;
            case 'o': {
                std::string outType = optarg;
                if (outType == "dot") {
                    outDot = true;
                } else if (outType == "csv") {
                    outCsv = true;
                }
                break;
            }
            case 1:
                grammarType = "LL(1)";
                parser = std::make_shared<mcc::LL1Parser>(grammar);
                break;
            case 2:
                grammarType = "SLR(1)";
                parser = std::make_shared<mcc::SLRParser>(grammar);
                break;
            case 3:
                grammarType = "LR(1)";
                parser = std::make_shared<mcc::LR1Parser>(grammar);
                break;
            default:
                usage();
                exit(1);
        }
    }

    if (optind < argc) {
        std::cout << "unknown arguments:";
        while (optind < argc) {
            std::cout << " " << argv[optind++];
        }
        std::cout << std::endl;
    }

    if (grammarFilepath.size() <= 0UL) {
        usage();
        exit(1);
    }
    if (auto ret = grammar.ReadFromFile(grammarFilepath); ret != 0) {
        util::LOG_ERROR("[ERROR] failed to read source grammar file");
        exit(1);
    }
    if (parser == nullptr || grammarType == "") {
        util::LOG_ERROR(
            "[ERROR] must select a grammar type in LL(1), SLR(1) and LR(1). the option is `--ll1`, `--slr` or "
            "`--lr1`");
        exit(1);
    }

    if (auto ret = parser->Parse(); ret != 0) {
        util::LOG_ERROR("[ERROR] invalid %s grammar, parsing failed", grammarType.c_str());
    } else {
        util::LOG_INFO("[INFO] parsed succeed. the grammar is a valid %s grammar", grammarType.c_str());
    }

    // parser->ShowDetails(std::cout);

    if (inputFilepath.size() > 0UL) {
        std::ifstream ifs(inputFilepath);
        if (!ifs) {
            util::LOG_ERROR("error: failed to open file '%s'.", inputFilepath.c_str());
            return -1;
        }
        parser->Analyze(ifs);
        ifs.close();
    } else {
        parser->Analyze(std::cin);
    }

    auto filename = std::string(::basename(grammarFilepath.c_str()));

    if (outDot) {
        auto dotFile = std::ofstream(filename + ".dot");
        parser->OutputToGraphviz(dotFile);
        dotFile.close();
    }

    if (outCsv) {
        auto csvFile = std::ofstream(filename + ".csv");
        parser->OutputToCsv(csvFile);
        csvFile.close();
    }

    return 0;
}
