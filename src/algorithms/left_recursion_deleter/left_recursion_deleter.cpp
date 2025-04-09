#include "left_recursion_deleter.h"


bool LRDeleter::isLeftRecursion(const std::string& nonterm, const std::vector<std::string>& prod)
{
    return !prod.empty() && prod[0] == nonterm;
}


void LRDeleter::removeLeftRecursion(GrammarInfo& grammar)
{
    std::vector<std::vector<std::string>> alpha;
    std::vector<std::vector<std::string>> beta;

    for (const auto& nonterm : grammar.getNonterm())
    {
        for (const auto& prod : (grammar.getRight())[nonterm])
        {
            if (isLeftRecursion(nonterm, prod))
            {

            }
        }
    }
}
