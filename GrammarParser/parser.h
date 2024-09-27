/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:10:36
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Color {
public:
    static const std::string NONE;
    static const std::string BLUE;
    static const std::string GREEN;
    static const std::string RED;
    static const std::string WHITE;
    static const std::string YELLOW;
};

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &right) : Left(left), Right(right) {}
    Production(std::string &&left, std::vector<std::string> &&right) : Left(left), Right(right) {}

public:
    std::string ToString() const;

public:
    std::string Left;
    std::vector<std::string> Right;
};

class Grammar {
public:
    std::vector<std::shared_ptr<Production>> Productions;
    std::map<std::string, std::vector<std::shared_ptr<Production>>> ProductionIndexes;
};

int ReadGrammar(std::string filepath, Grammar &grammar);
