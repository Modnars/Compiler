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
        : left_(left), right_(right), number_(number) { }

public:
    std::string ToString() const;

    bool operator<(const Production &rhs) const { return number_ < rhs.number_; }

public:
    const std::string &Left() const { return left_; }

    const std::vector<std::string> &Right() const { return right_; }

    std::int32_t Number() const { return number_; }

private:
    std::string left_;
    std::vector<std::string> right_;

    std::int32_t number_ = 0U;
};

class Grammar {
public:
    static const std::string EndMark;
    static const std::string NilMark;

public:
    /// @brief Read the productions from source grammar file. Seralize the text to `Production` objects.
    /// @return error code while reading the source file, parsing the productions.
    int ReadFromFile(const std::string &filepath);

    /// @brief Compute non-terminal symbols' FIRST set and cache the result in member `firstSet_`. If you need using
    /// grammar's FIRST set, must call the function first.
    void ComputeAndCacheFirstSet();

    /// @brief Compute non-terminal symbols' FOLLOW set and cache the result in member `followSet_`. If you need using
    /// grammar's FOLLOW set, must call the function first.
    void ComputeAndCacheFollowSet();

    /// @brief If a symbol appear in the production left part, it's a non-terminal symbol.
    bool IsNonTerminal(const std::string &symbol) const {
        return productionIndexes_.find(symbol) != productionIndexes_.end();
    }

    /// @brief Get all productions which defined in source grammar file.
    const std::vector<std::shared_ptr<const Production>> &AllProductions() const { return productions_; }

    /// @brief Get all productions whose left part is `symbol`.
    const std::vector<std::shared_ptr<Production>> &GetProductions(const std::string &symbol) const {
        static const std::vector<std::shared_ptr<Production>> empty;
        if (auto iter = productionIndexes_.find(symbol); iter != productionIndexes_.end()) {
            return iter->second;
        }
        return empty;
    }

    /// @brief Get cached target non-terminal symbol's FIRST set. You should call ComputeAndCacheFirsetSet before using
    /// the funciton.
    /// @warning Only return the NON-terminal symbols' FIRST set, for the terminal symbols, their FIRST set is
    /// themselves.
    const std::set<std::string> &FIRST(const std::string &symbol) const {
        static const std::set<std::string> empty;
        if (auto iter = firstSet_.find(symbol); iter != firstSet_.end()) {
            return iter->second;
        }
        return empty;
    }

    /// @brief Get cached target non-terminal symbol's FOLLOW set. You should call ComputeAndCacheFollowSet before using
    /// the funciton.
    /// @warning Only the NON-terminal symbols have FOLLOW set.
    const std::set<std::string> &FOLLOW(const std::string &symbol) const {
        static const std::set<std::string> empty;
        if (auto iter = followSet_.find(symbol); iter != followSet_.end()) {
            return iter->second;
        }
        return empty;
    }

    /// @brief Get production with its unique number. Production's number is the line number (which starts from 1) in
    /// source grammar file normally. The function is usually used to reduce symbols while parsing symbols with querying
    /// ActionTable. Please note the difference from the GetProductions function.
    std::shared_ptr<const Production> GetProduction(std::int32_t number) const {
        if (number > static_cast<int32_t>(productions_.size())) {
            return nullptr;
        }
        return productions_[static_cast<std::size_t>(number - 1)];
    }

    /// @brief Compute the FirstSet for a SubFields for a production's right part.
    /// @warning When `@param begin` == `@param end`, the result is { Grammar::NilMark }.
    std::set<std::string> ComputeFirstSet(std::vector<std::string>::const_iterator begin,
                                           std::vector<std::string>::const_iterator end) const;

    /// @brief Print the brief information of the grammar.
    void ShowDetails(std::ostream &os) const;

private:
    bool canGenNil(const std::string &symbol) const {
        auto setIter = firstSet_.find(symbol);
        if (setIter == firstSet_.end()) {
            return false;
        }
        return setIter->second.find(Grammar::NilMark) != setIter->second.end();
    }

    std::set<std::string> computeAndCacheFirstSet(const std::string &symbol);

private:
    std::vector<std::shared_ptr<const Production>> productions_;
    std::map<std::string, std::vector<std::shared_ptr<Production>>> productionIndexes_;
    std::map<std::string, std::set<std::string>> firstSet_;
    std::map<std::string, std::set<std::string>> followSet_;
};

int ReadGrammar(std::string filepath, Grammar &grammar);
