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
    Production(std::string &&left, std::vector<std::string> &&right, std::int32_t number)
        : Left(left), Right(right), number_(number) { }

public:
    std::string ToString() const;

public:
    std::string Left;
    std::vector<std::string> Right;

    std::int32_t Number() const { return number_; }

private:
    std::int32_t number_ = 0U;
};

class Grammar {
public:
    static const std::string EndMark;
    static const std::string NilMark;

public:
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

    std::shared_ptr<const Production> GetProduction(std::int32_t number) const {
        if (number > static_cast<int32_t>(Productions.size())) {
            return nullptr;
        }
        return Productions[static_cast<std::size_t>(number - 1)];
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
};

int ReadGrammar(std::string filepath, Grammar &grammar);
