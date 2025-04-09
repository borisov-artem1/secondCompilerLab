#include "grammar_info.h"

#include <fstream>
#include <vector>
#include <algorithm>

void GrammarInfo::parseInputFiles(const std::string& filename)
{
    std::string path = "../src/tests/input/" + filename;
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
    for (const auto& token : tokens[0])
    {
        s_nonterm.emplace_back(&token);
    }
    for (const auto& token : tokens[1])
    {
        s_term.emplace_back(&token);
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

std::string GrammarInfo::getStartNonterm() const
{
    return start_nonterm;
}

std::vector<std::string> GrammarInfo::getNonterm() const
{
    return s_nonterm;
}

std::map<std::string, std::vector<std::vector<std::string>>> GrammarInfo::getRight() const
{
    return s_rights;
}

std::vector<std::string> GrammarInfo::getTerm() const
{
    return s_term;
}





