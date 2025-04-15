#include "left_recursion_deleter.h"
#include <fstream>
#include <unordered_set>
#include <algorithm>
//
bool LRDeleter::isLeftRecursion(const std::string& nonterm, const std::vector<std::string>& prod)
{
    return !prod.empty() && prod[0] == nonterm;
}


void LRDeleter::removeLeftRecursion(GrammarInfo& grammar)
{
    std::map<std::string, std::vector<std::vector<std::string>>> newRules;
    std::vector<std::string> newNonterms = grammar.s_nonterm;

    for (const auto& A : grammar.s_nonterm)
    {
        std::vector<std::vector<std::string>> alpha;
        std::vector<std::vector<std::string>> beta;

        for (const auto& prod : grammar.s_rights[A])
        {
            if (!prod.empty() && prod[0] == A)
            {
                std::vector<std::string> suffix(prod.begin() + 1, prod.end());
                suffix.push_back(A + "1");
                alpha.push_back(suffix);
            }
            else
            {
                std::vector<std::string> newBeta = prod;
                if (!alpha.empty())
                    newBeta.push_back(A + "1");
                beta.push_back(newBeta);
            }
        }

        if (!alpha.empty())
        {
            newRules[A] = beta;
            alpha.push_back({ "ε" });
            newRules[A + "1"] = alpha;
            newNonterms.push_back(A + "1");
        }
        else
        {
            newRules[A] = grammar.s_rights[A];
        }
    }

    grammar.s_rights = newRules;
    grammar.s_nonterm = newNonterms;
    //removeEpsilonRules(grammar);
}

void LRDeleter::printGrammar(const GrammarInfo& grammar, const std::string& nameDir, int&& counter)
{
    //static int counter = 1;
    std::string pathToFile = "../src/tests/output" + nameDir + "/" + std::to_string(counter++) + "_output.txt";
    std::ofstream file(pathToFile);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + pathToFile);
    }

    file << grammar.s_nonterm.size() << "\n";
    for (const auto& nt : grammar.s_nonterm) {
        file << nt << " ";
    }
    file << "\n";

    // 2. Количество и список терминалов
    file << grammar.s_term.size() << "\n";
    for (const auto& t : grammar.s_term) {
        file << t << " ";
    }
    file << "\n";

    size_t ruleCount = 0;
    for (const auto& [lhs, rhsList] : grammar.s_rights) {
        ruleCount += rhsList.size();
    }
    file << ruleCount << "\n";

    for (const auto& [lhs, rhsList] : grammar.s_rights) {
        for (const auto& rhs : rhsList) {
            file << lhs << " -> ";
            for (const auto& sym : rhs) {
                file << sym << " ";
            }
            file << "\n";
        }
    }

    file << grammar.start_nonterm << "\n";

    file.close();
}

std::set<std::string> LRDeleter::findNullable(const GrammarInfo& grammar)
{
    std::set<std::string> nullable;
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (const auto& [nt, prods] : grammar.s_rights)
        {
            for (auto& prod : prods)
            {
                if (prod.size() == 1 && prod[0] == "ε")
                {
                    if (nullable.insert(nt).second)
                    {
                        changed = true;
                    }
                } else if (std::ranges::all_of(prod, [&](const std::string& sym)
                {
                    return nullable.count(sym);
                }))
                {
                    if (nullable.insert(nt).second)
                    {
                        changed = true;
                    }
                }
            }
        }
    }

    return nullable;
}


void LRDeleter::removeEpsilonRules(GrammarInfo& grammar)
{
    std::set<std::string> nullable = findNullable(grammar);
    std::map<std::string, std::vector<std::vector<std::string>>> newGrammar;

    for (const auto& [nt, prods] : grammar.s_rights)
    {
        for (const auto& prod : prods)
        {
            if (prod.size() == 1 && prod[0] == "ε") continue;

            size_t n = prod.size();
            int total = 1 << n;

            for (int mask = 0; mask < total; ++mask)
            {
                std::vector<std::string> newProd;
                for (int i = 0; i < n; ++i)
                {
                    if (!(mask & (1 << i)) || !nullable.contains(prod[i]))
                    {
                        newProd.push_back(prod[i]);
                    }
                }
                if (!newProd.empty())
                {
                    bool exists = std::any_of(newGrammar[nt].begin(), newGrammar[nt].end(),
                        [&](const std::vector<std::string>& existing)
                    {
                            return existing == newProd;
                    });
                    if (!exists)
                    {
                        newGrammar[nt].push_back(newProd);
                    }
                }
            }
        }
    }

    if (nullable.contains(grammar.start_nonterm))
    {
        newGrammar[grammar.start_nonterm].push_back({"ε"});
    }
    grammar.s_rights = newGrammar;
}

// void LRDeleter::printToDirs()
// {
//     GrammarInfo grammar_info;
//
//     grammar_info.parseInputFiles("2_input.txt");
//     LRDeleter::removeLeftRecursion(grammar_info);
//     //ChainRulesDeleter::removeChainRules(grammar_info);
//     LRDeleter::printGrammar(grammar_info);
// }
