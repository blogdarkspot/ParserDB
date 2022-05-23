#include "grammar/pcfg.hpp"
#include "grammar/cky.hpp"
#include "rules.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

std::shared_ptr<grammar::cfg::ProbabilisticRule> _parser(const std::wstring &rule);
std::shared_ptr<grammar::cfg::ProbabilisticRule> add_terminal(std::wstring left, std::wstring right);
void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol> node, bool isLeft);
void printBT(const std::shared_ptr<grammar::cfg::symbol> node);


TEST(Grammar, InputRules)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG>();
    auto _cky = std::make_shared<grammar::parser::cky>();
    
    _pcfg->set_rules(_parser(L"S -> A"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_start_symbol(L"S");
    std::vector<std::wstring> tokens = {L"a"};
    
    _cky->set_cfg(_pcfg);

    auto trees = _cky->get_trees(tokens);

    EXPECT_GT(trees.size(), 0);

    auto tree = trees[0];
}

TEST(Grammar, TwoLevels)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG>();
    auto _cky = std::make_shared<grammar::parser::cky>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"B", L"b"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

}

TEST(Grammar, TreeLevels)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG>();
    auto _cky = std::make_shared<grammar::parser::cky>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_rules(_parser(L"B -> C D"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"C", L"b"));
    _pcfg->set_terminals(add_terminal(L"D", L"c"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"c", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

}

TEST(Grammar, Recursive)
{
    auto _pcfg = std::make_shared<grammar::cfg::PCFG>();
    auto _cky = std::make_shared<grammar::parser::cky>();
    
    _pcfg->set_rules(_parser(L"S -> A B"));
    _pcfg->set_rules(_parser(L"B -> C D"));
    _pcfg->set_rules(_parser(L"B -> B B"));
    _pcfg->set_terminals(add_terminal(L"A", L"a"));
    _pcfg->set_terminals(add_terminal(L"C", L"b"));
    _pcfg->set_terminals(add_terminal(L"D", L"c"));
    _pcfg->set_start_symbol(L"S");
    _cky->set_cfg(_pcfg);
    {
        std::vector<std::wstring> tokens = {L"b", L"c", L"a"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 0);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
    }

    {
        std::vector<std::wstring> tokens = {L"a", L"b", L"c", L"b", L"c", L"b", L"c"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_GT(trees.size(), 1);
    }

}

class PCFGTest : public ::testing::Test {
    protected:
      void SetUp() override
      {
          _pcfg = std::make_shared<grammar::cfg::PCFG>();

          _pcfg->set_rules(_parser(L"DP -> DET NP"));
          _pcfg->set_rules(_parser(L"DP -> DET PossP"));
          _pcfg->set_rules(_parser(L"DP -> DET NumP"));

          _pcfg->set_rules(_parser(L"NP -> N"));
          _pcfg->set_rules(_parser(L"NP -> NP AP"));
          _pcfg->set_rules(_parser(L"NP -> AP NP"));
          _pcfg->set_rules(_parser(L"NP -> NP PP"));
          _pcfg->set_rules(_parser(L"NP -> NP CP"));

          _pcfg->set_rules(_parser(L"NumP -> Num NP"));
          _pcfg->set_rules(_parser(L"NumP -> Num PP"));

          _pcfg->set_rules(_parser(L"PossP -> Poss NP"));
          _pcfg->set_rules(_parser(L"PossP -> NP Poss"));
          _pcfg->set_rules(_parser(L"PossP -> Poss NumP"));

          _pcfg->set_rules(_parser(L"VP -> AdvP V"));
          _pcfg->set_rules(_parser(L"VP -> V AdvP"));
          _pcfg->set_rules(_parser(L"VP -> V"));
          _pcfg->set_rules(_parser(L"VP -> V DP"));
          _pcfg->set_rules(_parser(L"VP -> V PP"));
          _pcfg->set_rules(_parser(L"VP -> V CP"));
          _pcfg->set_rules(_parser(L"VP -> V AP"));
          _pcfg->set_rules(_parser(L"VP -> V AdvP"));
          _pcfg->set_rules(_parser(L"VP -> V PP"));
          _pcfg->set_rules(_parser(L"VP -> V IP"));

          

          _pcfg->set_rules(_parser(L"IP -> DP'IP"));
          _pcfg->set_rules(_parser(L"IP -> DP IP"));
          _pcfg->set_rules(_parser(L"IP -> DP VP"));
          _pcfg->set_rules(_parser(L"IP -> NP 'IP"));
          _pcfg->set_rules(_parser(L"IP -> NP IP"));
          _pcfg->set_rules(_parser(L"IP -> NP VP"));
          _pcfg->set_rules(_parser(L"IP -> PossP 'IP"));
          _pcfg->set_rules(_parser(L"IP -> PossP IP"));
          _pcfg->set_rules(_parser(L"IP -> PossP VP"));
          _pcfg->set_rules(_parser(L"IP -> NumP 'IP"));
          _pcfg->set_rules(_parser(L"IP -> NumP IP"));
          _pcfg->set_rules(_parser(L"IP -> NumP VP"));
          _pcfg->set_rules(_parser(L"'IP -> I VP"));
          _pcfg->set_rules(_parser(L"'IP -> I IP"));

          _pcfg->set_rules(_parser(L"CP -> C IP"));

          _pcfg->set_rules(_parser(L"AP -> A"));
          _pcfg->set_rules(_parser(L"AP -> AdvP AP"));
          _pcfg->set_rules(_parser(L"AP -> AP AdvP"));
          _pcfg->set_rules(_parser(L"AP -> AP PP"));
          _pcfg->set_rules(_parser(L"AP -> A PP"));
          _pcfg->set_rules(_parser(L"AP -> A CP"));

         
          _pcfg->set_rules(_parser(L"PP -> AdvP PP"));
          _pcfg->set_rules(_parser(L"PP -> P DP"));
          _pcfg->set_rules(_parser(L"PP -> P AdvP"));
          _pcfg->set_rules(_parser(L"PP -> P PP"));
          _pcfg->set_rules(_parser(L"PP -> P PP"));

          _pcfg->set_rules(_parser(L"AdvP -> ADV"));
          _pcfg->set_rules(_parser(L"AdvP -> AdvP AdvP"));
          _pcfg->set_rules(_parser(L"AdvP -> ADV PP"));

          

          _pcfg->set_start_symbol(L"IP");
      }

    void TearDown() override {

    }

    std::shared_ptr<grammar::cfg::PCFG> _pcfg;
};

/*
_pcfg->set_rules(_parser(L"DP -> DET NP"));
_pcfg->set_rules(_parser(L"DP -> DET PossP"));
_pcfg->set_rules(_parser(L"DP -> DET NumP"));
*/
TEST_F(PCFGTest, DPRules)
{
    auto _cky = std::make_shared<grammar::parser::cky>();

    _cky->set_cfg(_pcfg);

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Sujeito", L"Verbo"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"Poss", L"Poss"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Poss", L"Sujeito", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"Num", L"Num"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Num", L"Sujeito", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }
}

/*
_pcfg->set_rules(_parser(L"NP -> N"));
_pcfg->set_rules(_parser(L"NP -> NP AP"));
_pcfg->set_rules(_parser(L"NP -> AP NP"));
_pcfg->set_rules(_parser(L"NP -> NP PP"));
_pcfg->set_rules(_parser(L"NP -> NP CP"));
*/
TEST_F(PCFGTest, NPRules)
{
    auto _cky = std::make_shared<grammar::parser::cky>();

    _cky->set_cfg(_pcfg);

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Sujeito", L"Verbo"};
        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"A", L"Adjetivo"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Adjetivo", L"Sujeito", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"A", L"Adjetivo"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Sujeito", L"Adjetivo", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"PREP", L"Preposicao"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Sujeito", L"Preposicao", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        _pcfg->clear_terminals();
    }

    {
        _pcfg->set_terminals(add_terminal(L"N", L"Sujeito"));
        _pcfg->set_terminals(add_terminal(L"C", L"Conjuncao"));
        _pcfg->set_terminals(add_terminal(L"DET", L"Det"));
        _pcfg->set_terminals(add_terminal(L"V", L"Verbo"));

        std::vector<std::wstring> tokens = {L"Det", L"Sujeito", L"Conjuncao", L"Sujeito", L"Verbo", L"Verbo"};

        auto trees = _cky->get_trees(tokens);
        EXPECT_EQ(trees.size(), 1);
        printBT(trees[0]);
        _pcfg->clear_terminals();
    }

}

std::shared_ptr<grammar::cfg::ProbabilisticRule> add_terminal(std::wstring left, std::wstring right)
{
    std::vector<std::wstring> _right;
    _right.emplace_back(right);
    return std::make_shared<::grammar::cfg::ProbabilisticRule>(left, _right, true);
}

std::shared_ptr<grammar::cfg::ProbabilisticRule> _parser(const std::wstring &rule)
{
    auto pos = rule.find_first_of(L"-");

    std::wstring left;
    std::vector<std::wstring> right;

    if (pos == rule.npos)
    {
        return nullptr;
    }
    if (rule[pos + 1] != L'>')
    {
        return nullptr;
    }

    left = rule.substr(0, pos - 1);
    pos += 3;

    do
    {
        auto tmp = rule.find_first_of(L' ', pos);

        if (tmp == rule.npos)
        {
            right.emplace_back(rule.substr(pos));
            break;
        }
        else
        {
            auto s = rule.substr(pos, tmp - pos);
            right.emplace_back(s);
            pos += (tmp - pos) + 1; 
        }
    } while (true);

    return std::make_shared<::grammar::cfg::ProbabilisticRule>(left, right, false);
}

void printBT(const std::wstring& prefix, const std::shared_ptr<grammar::cfg::symbol> node, bool isLeft)
{
    if( node != nullptr )
    {
        std::wcout << prefix;

        std::wcout << (isLeft ? L"├──" : L"└──" );

        bool is_terminal = node->is_terminal;
        // print the value of the node
        std::wcout << L" p: " << node->probability << L" " << node->value <<  (is_terminal ? L"── " : L"\n");

        if(is_terminal)
        {
            std::wcout << (std::static_pointer_cast<grammar::cfg::lexicon>(node))->lex << std::endl;
        }
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->left, true);
        printBT( prefix + (isLeft ? L"│   " : L"    "), node->right, false);
    }
}

void printBT(const std::shared_ptr<grammar::cfg::symbol> node)
{
    printBT(L"", node, false);    
}