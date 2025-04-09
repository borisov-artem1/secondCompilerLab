#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>

class GrammarInfo {
    std::vector<std::string> s_term;
    std::vector<std::string> s_nonterm;
    std::map<std::string, std::vector<std::vector<std::string>>> s_rights;
    std::string start_nonterm;
public:
    void parseInputFiles(const std::string& filename);
    [[nodiscard]] std::vector<std::vector<std::string>> getRights(const std::string& stringRights) const;
    [[nodiscard]] std::string getStartNonterm() const;
    [[nodiscard]] std::vector<std::string> getNonterm() const;
    [[nodiscard]] std::map<std::string, std::vector<std::vector<std::string>>> getRight() const;
    [[nodiscard]] std::vector<std::string> getTerm() const;
};


