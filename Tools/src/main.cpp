#include <iostream>
#include <cstdlib>

#include "GrammaAnalysis.hpp"

int main(int argc, const char *argv[]) {
    std::vector<std::string> rights;
    rights.push_back("1");
    rights.push_back("2");
    rights.push_back("3");
    Production p("left", rights);
    std::cout << p << std::endl;
    rights.push_back("4");
    std::cout << p << std::endl;
    return EXIT_SUCCESS;
}
