#include <bits/stdc++.h>

class L_System{

private:
    std::string axioms;
    std::vector<std::string> alphabets;
    std::vector<std::string> symbols;
    std::unordered_map<std::string, double> constants;
    std::vector<std::string> production_rules;

public:
    L_System(std::string axioms_, std::vector<std::string> alphabets_, std::unordered_map<std::string, double> constants_, std::vector<std::string> production_rules_, std::vector<std::string> symbols_):
        axioms(axioms_), alphabets(alphabets_), constants(constants_), symbols(symbols_), production_rules(production_rules_) {}
    
    std::string generateWord(const int steps) const;
    std::string next_step(std::string& word) const;
    std::vector<std::string> generateModule(std::string& word) const;
    std::vector<std::string> split_rule(std::string rule) const;
    int num_param(std::string module) const;
    bool matching(std::string module, std::string rule) const;
    std::string expand(std::string module, std::string rule) const;
    bool condition_check(std::string condition, std::unordered_map<std::string, double> params) const;
    std::unordered_map<std::string, double> variable_value_mapping(std::string rule, std::string module) const;
    std::string expand(std::string module, std::string rule) const;
};