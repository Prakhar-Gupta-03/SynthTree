#include "l_system.h"

std::vector<std::string> L_System::generateModule(std::string& word) const {
    std::vector<std::string> modules;
    std::string module = "";
    for(int i = 0; i<word.size(); i++){
        if (std::find(alphabets.begin(), alphabets.end(), std::string(1, word[i])) != alphabets.end() || std::find(symbols.begin(), symbols.end(), std::string(1, word[i])) != symbols.end()){
            if(module != ""){
                modules.push_back(module);
            }
            module = word[i];
        }
        else{
            module += word[i];
        }
    }
    if(module != ""){
        modules.push_back(module);
    }
    return modules;
}

std::vector<std::string> L_System::split_rule(std::string rule) const {
    size_t index1 = rule.find(':'), index2 = rule.find('→');
    return {rule.substr(0, index1), rule.substr(index1+1, index2-index1-1), rule.substr(index2+1)};
}

int L_System::num_param(std::string module) const {
    if(module.find("()") != std::string::npos){
        return 0;
    }
    return std::count(module.begin(), module.end(), ',') + 1;
}

bool L_System::matching(std::string module, std::string splitted_rule) const {

    return splitted_rule[0]==module[0] && num_param(splitted_rule) == num_param(module);
}

bool condition_check(std::string rule, std::string condition, std::string module){
    return true;
}

std::string expand(std::string module, std::string rule){
    return "";
}

std::string L_System::next_step(std::string& word) const {
    std::string result = "";
    std::vector<std::string> modules = generateModule(word);
    for(int i = 0; i<modules.size(); i++){
        std::string module = modules[i];
        if(std::find(symbols.begin(), symbols.end(), module) != symbols.end()){
            result += module;
        }else{
            for(int j = 0; j<production_rules.size(); j++){
                std::vector<std::string> splitted_production_rule = split_rule(production_rules[j]);
                if(matching(module, production_rules[j]) && condition_check(splitted_production_rule[0], splitted_production_rule[1], module)){
                    result += expand(module, production_rules[j]);
                }
            }
        }
    }
    return result;
}



std::string L_System::generateWord(const int steps) const{
    std::string word = axioms;
    for (int i = 0; i<steps; i++){
        word = next_step(word);
    }
    return word;
}