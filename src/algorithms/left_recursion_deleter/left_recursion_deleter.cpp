#include "left_recursion_deleter.h"
#include <fstream>
#include <unordered_set>
#include <algorithm>
//
bool LRDeleter::isLeftRecursion(const std::string& nonterm, const std::vector<std::string>& prod)
{
    return !prod.empty() && prod[0] == nonterm;
}
//removeEpsilonRules(grammar);

void LRDeleter::removeLeftRecursion(GrammarInfo& grammar)
{
    auto& nonterminals = grammar.s_nonterm;
    auto& productions = grammar.s_rights;
    std::map<std::string, std::vector<std::vector<std::string>>> new_productions;
    std::vector<std::string> new_nonterminals = nonterminals;

    for (size_t i = 0; i < nonterminals.size(); ++i)
    {
        const std::string& Ai = nonterminals[i];

        // Шаг 1: Заменить Ai → Aj γ, где j < i
        for (size_t j = 0; j < i; ++j)
        {
            const std::string& Aj = nonterminals[j];
            std::vector<std::vector<std::string>> updated_rules;

            for (const auto& rule : productions[Ai])
            {
                if (!rule.empty() && rule[0] == Aj)
                {
                    for (const auto& Aj_rule : new_productions[Aj])
                    {
                        std::vector<std::string> new_rule = Aj_rule;
                        new_rule.insert(new_rule.end(), rule.begin() + 1, rule.end());
                        updated_rules.push_back(new_rule);
                    }
                }
                else
                {
                    updated_rules.push_back(rule);
                }
            }

            productions[Ai] = updated_rules;
        }

        // Шаг 2: Устранить непосредственную левую рекурсию у Ai
        std::vector<std::vector<std::string>> alpha;
        std::vector<std::vector<std::string>> beta;

        for (const auto& rule : productions[Ai])
        {
            if (!rule.empty() && rule[0] == Ai)
            {
                std::vector<std::string> suffix(rule.begin() + 1, rule.end());
                suffix.push_back(Ai + "'");
                alpha.push_back(suffix);
            }
            else
            {
                std::vector<std::string> new_beta = rule;
                if (!alpha.empty())
                    new_beta.push_back(Ai + "'");
                beta.push_back(new_beta);
            }
        }

        if (!alpha.empty())
        {
            new_productions[Ai] = beta;
            alpha.push_back({ "ε" });
            new_productions[Ai + "'"] = alpha;

            new_nonterminals.push_back(Ai + "'");
        }
        else
        {
            new_productions[Ai] = productions[Ai];
        }
    }

    grammar.s_rights = new_productions;
    grammar.s_nonterm = new_nonterminals;
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
