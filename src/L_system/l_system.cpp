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
    size_t index1 = rule.find(':'), index2 = rule.find('?');
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

std::unordered_map<std::string, double> L_System::variable_value_mapping(std::string rule, std::string module) const {
    std::unordered_map<std::string, double> params;
    size_t index1 = rule.find('('), index2 = rule.find(',');
    size_t index3 = module.find('('), index4 = module.find(',');
    while(index1 != std::string::npos){
        std::string variable = rule.substr(index1+1, index2-index1-1), value = module.substr(index3+1, index4-index3-1);
        if(variable[variable.length()-1]==')'){
            variable = variable.substr(0, variable.length()-1);
            value = value.substr(0, value.length()-1);
        }
        params[variable] = std::stod(value);
        index1 = rule.find(',', index1+1);
        index2 = rule.find(',', index2+1);
        index3 = module.find(',', index3+1);
        index4 = module.find(',', index4+1);
    }
    for(auto itr = constants.begin(); itr != constants.end(); itr++){
        params[itr->first] = itr->second;
    }
    return params;
}

bool L_System::condition_check(std::string condition, std::unordered_map<std::string, double> params) const {
    if(condition == ""){
        return true;
    }

    std::string condition_rule = condition;
    for(auto it = params.begin(); it != params.end(); it++){
        size_t index = condition_rule.find(it->first);
        while(index != std::string::npos){
            condition_rule.replace(index, it->first.size(), std::to_string(it->second));
            index = condition_rule.find(it->first);
        }
    }

    if(condition_rule.find("==") != std::string::npos){
        size_t index = condition_rule.find("==");
        return std::stod(condition_rule.substr(0, index)) == std::stod(condition_rule.substr(index+2));
    }
    else if(condition_rule.find("!=") != std::string::npos){
        size_t index = condition_rule.find("!=");
        return std::stod(condition_rule.substr(0, index)) != std::stod(condition_rule.substr(index+2));
    }
    else if(condition_rule.find(">=") != std::string::npos){
        size_t index = condition_rule.find(">=");
        return std::stod(condition_rule.substr(0, index)) >= std::stod(condition_rule.substr(index+2));
    }
    else if(condition_rule.find("<=") != std::string::npos){
        size_t index = condition_rule.find("<=");
        return std::stod(condition_rule.substr(0, index)) <= std::stod(condition_rule.substr(index+2));
    }
    else if(condition_rule.find(">") != std::string::npos){
        size_t index = condition_rule.find(">");
        return std::stod(condition_rule.substr(0, index)) > std::stod(condition_rule.substr(index+1));
    }
    else if(condition_rule.find("<") != std::string::npos){
        size_t index = condition_rule.find("<");
        return std::stod(condition_rule.substr(0, index)) < std::stod(condition_rule.substr(index+1));
    }
    return false;
}

std::string L_System::expand(std::string module, std::string rule) const {
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
                std::unordered_map <std::string, double> params = variable_value_mapping(splitted_production_rule[0], module);
                if(matching(module, splitted_production_rule[0]) && condition_check(splitted_production_rule[1], params)){
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