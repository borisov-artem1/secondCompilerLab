#include "grammar_info.h"

#include <fstream>
#include <vector>
#include <algorithm>
//#include <bits/fs_dir.h>
#include <filesystem>

#include "../algorithms/chain_rules_deleter/chain_rules_deleter.h"
#include "../algorithms/left_recursion_deleter/left_recursion_deleter.h"

void GrammarInfo::parseInputFiles(const std::string& path)
{
    //std::string path = "../src/tests/input/" + filename;
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + path);
    }
    std::string line;
    std::vector<std::string> tokens;
    int counter = 0;
    while (std::getline(file, line))
    {
        counter++;
        if (!std::isdigit(line[0])) {
            line.erase(std::ranges::remove(line, ' ').begin(), line.end());
            tokens.push_back(line);
        }
    }
    for (auto& token : tokens[0])
    {
        s_nonterm.emplace_back(1, token);
    }
    for (auto& token : tokens[1])
    {
        s_term.emplace_back(1, token);
    }
    for (int i = 2; i < tokens.size(); i++)
    {
        if (i == tokens.size() - 1)
        {
            start_nonterm = tokens[i];
            break;
        }
        auto NonTerm = std::string(1, tokens[i][0]);
        std::vector<std::vector<std::string>> Term = getRights(tokens[i].substr(3, tokens[i].size()));
        if (s_rights.contains(NonTerm))
        {
            s_rights[NonTerm].insert(s_rights[NonTerm].end(), Term.begin(), Term.end());
        } else
        {
            s_rights[NonTerm] = Term;
        }
    }
    file.close();
}

std::vector<std::vector<std::string>> GrammarInfo::getRights(const std::string& stringRights) const
{
    std::vector<std::vector<std::string>> rights;
    std::vector<std::string> current;

    for (const char& c : stringRights)
    {
        if (c == '|')
        {
            if (!current.empty())
            {
                rights.push_back(current);
                current.clear();
            }
        } else
        {
            current.emplace_back(1, c);
        }
    }

    if (!current.empty())
    {
        rights.push_back(current);
    }

    return rights;
}

void GrammarInfo::parseDir()
{
    std::string pathToDir = "../src/tests/input/";
    int LRcounter = 1;
    int chainCounter = 1;

    for (auto& entry : std::filesystem::directory_iterator(pathToDir))
    {
        if (entry.is_regular_file())
        {
            parseInputFiles(entry.path());
            LRDeleter::removeLeftRecursion(*this);
            LRDeleter::printGrammar(*this, "LRDelete", LRcounter++);
            ChainRulesDeleter::removeChainRules(*this);
            LRDeleter::printGrammar(*this, "ChainRulesDelete", chainCounter++);
        }
    }
}

