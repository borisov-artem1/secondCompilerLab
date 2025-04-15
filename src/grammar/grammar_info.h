#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>

struct GrammarInfo {
    std::vector<std::string> s_term;
    std::vector<std::string> s_nonterm;
    std::map<std::string, std::vector<std::vector<std::string>>> s_rights;
    std::string start_nonterm;
    void parseInputFiles(const std::string& filename);
    void parseDir();
    [[nodiscard]] std::vector<std::vector<std::string>> getRights(const std::string& stringRights) const;

};


