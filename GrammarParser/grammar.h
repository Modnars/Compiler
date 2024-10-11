/*
 * @Author: modnarshen
 * @Date: 2024.10.08 15:33:07
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &right)
        : Left(left), Right(right), number(++numberCounter) { }
    Production(std::string &&left, std::vector<std::string> &&right)
        : Left(left), Right(right), number(++numberCounter) { }

public:
    std::string ToString() const;

public:
    std::string Left;
    std::vector<std::string> Right;

private:
    std::uint32_t number = 0U;
    static std::uint32_t numberCounter;
};

class Grammar {
public:
    static const std::string EndMark;
    static const std::string NilMark;

public:
    int CalcFirstSet();
    int ComputeFirstSet();

    bool IsTerminal(const std::string &symbol) const {
        return this->ProductionIndexes.find(symbol) == this->ProductionIndexes.end();
    }

    const std::vector<std::shared_ptr<Production>> &GetProductions(const std::string &symbol) const {
        static const std::vector<std::shared_ptr<Production>> empty;
        if (auto iter = this->ProductionIndexes.find(symbol); iter != this->ProductionIndexes.end()) {
            return iter->second;
        }
        return empty;
    }

public:
    std::vector<std::shared_ptr<Production>> Productions;
    std::map<std::string, std::vector<std::shared_ptr<Production>>> ProductionIndexes;
    std::map<std::string, std::set<std::string>> FirstSet;

private:
    bool canGenNil(const std::string &symbol) const {
        auto setIter = this->FirstSet.find(symbol);
        if (setIter == this->FirstSet.end()) {
            return false;
        }
        return setIter->second.find(Grammar::NilMark) != setIter->second.end();
    }

    std::set<std::string> computeFirstSet(const std::string &symbol);
    std::set<std::string> computeFirstSet(std::shared_ptr<Production> production);
};

int ReadGrammar(std::string filepath, Grammar &grammar);
