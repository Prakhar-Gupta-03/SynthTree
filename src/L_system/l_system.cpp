#include "l_system.h"

// Split the word string into modules where each module is a single instruction for the turtle
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

// Split the production rule into 3 parts: the module, the condition and the expression
std::vector<std::string> L_System::split_rule(std::string rule) const {
    size_t index1 = rule.find(':'), index2 = rule.find('?');
    return {rule.substr(0, index1), rule.substr(index1+1, index2-index1-1), rule.substr(index2+1)};
}

// Count the number of parameters in a module
int L_System::num_param(std::string module) const {
    if(module.find("()") != std::string::npos){
        return 0;
    }
    return std::count(module.begin(), module.end(), ',') + 1;
}

// Check if the module and the production rule match
bool L_System::matching(std::string module, std::string splitted_rule) const {

    return splitted_rule[0]==module[0] && num_param(splitted_rule) == num_param(module);
}

// Create a mapping from the variables in the production rule to the values in the module
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

// Check if the condition is true or false
bool L_System::condition_check(std::string condition, std::unordered_map<std::string, double> params) const {
    if(condition == "True"){
        return true;
    }else if(condition == "False"){
        return false;
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


// Evaluate a mathematical expression with the given parameters
std::string L_System::eval(std::string expression, std::unordered_map<std::string, double> params) const {
    std::string result = expression;
    for(int i = 0; i<result.length(); i++){
        if(result[i] == '('){
            int j = i+1;
            int count = 1;
            while(count!=0){
                if(result[j] == '('){
                    count++;
                }else if(result[j] == ')'){
                    count--;
                }
                j++;
            }
            j--;
            std::string to_eval = result.substr(i+1, j-i-1);
            result = result.replace(i, j-i+1, eval(to_eval, params));
        }
    }
    for(int i = 0; i<result.length(); i++){   
        if(result[i]=='+' || result[i]=='-' || result[i]=='*' || result[i]=='/' || result[i]=='^'){
            int j = i-1;
            while(j>=0 && result[j]!='+' && result[j]!='-' && result[j]!='*' && result[j]!='/' && result[j]!='^'){
                j--;
            }
            std::string left = result.substr(j+1, i-j-1);
            int k = i+1;
            while(k<result.length() && result[k]!='+' && result[k]!='-' && result[k]!='*' && result[k]!='/' && result[k]!='^'){
                k++;
            }
            std::string right = result.substr(i+1, k-i-1);

            double left_value, right_value;
            try{
                left_value = std::stod(left);
            }
            catch (const std::invalid_argument&){
                left_value = params[left];
            }
            try{
                right_value = std::stod(right);
            }
            catch (const std::invalid_argument&){
                right_value = params[right];
            }
            if(result[i]=='+'){
                result = result.replace(j+1, k-j-1, std::to_string(left_value+right_value));
            }else if(result[i]=='-'){
                result = result.replace(j+1, k-j-1, std::to_string(left_value-right_value));
            }else if(result[i]=='*'){
                result = result.replace(j+1, k-j-1, std::to_string(left_value*right_value));
            }else if(result[i]=='/'){
                result = result.replace(j+1, k-j-1, std::to_string(left_value/right_value));
            }else if(result[i]=='^'){
                result = result.replace(j+1, k-j-1, std::to_string(pow(left_value, right_value)));
            }
            i = k-1;
        }
    }
    return result;
}

// Expand the expression by evaluating the mathematical expressions and replacing the variables with their values
std::string L_System::expand(std::string module, std::string expression, std::unordered_map<std::string, double> params) const {
    std::string result = expression;
    for(auto itr = params.begin(); itr != params.end(); itr++){
        size_t index = result.find(itr->first);
        while(index != std::string::npos){
            result.replace(index, itr->first.size(), std::to_string(itr->second));
            index = result.find(itr->first);
        }
    }
    std::stack<int> s;
    for(int i = 0; i<result.length(); i++){

        if(result[i] == '('){
            s.push(i);
        }
        else if(result[i] == ')' || result[i]==','){
            if(s.size()!=1){
                s.pop();
            }else{
                int index = s.top();
                s.pop();
                bool flag = false;
                if(result[i]==','){
                    flag = true;
                }
                std::string to_eval = result.substr(index+1, i-index-1);
                std::string eval_string = eval(to_eval, params);
                result = result.replace(index+1, to_eval.length(), eval_string);
                if(flag){
                    s.push(index + eval_string.length() + 1);
                }
                i = index + eval_string.length() + 2;
            }
        }
        

    }
    return result;
}

// Generate the next step of the L-System
std::string L_System::next_step(std::string& word) const {
    std::string result = "";
    std::vector<std::string> modules = generateModule(word);
    for(int i = 0; i<modules.size(); i++){
        std::string module = modules[i];
        if(std::find(symbols.begin(), symbols.end(), std::string(1, module[0])) != symbols.end()){
            result += module;
        }else{
            for(int j = 0; j<production_rules.size(); j++){
                std::vector<std::string> splitted_production_rule = split_rule(production_rules[j]);
                if(matching(module, splitted_production_rule[0])){
                    std::unordered_map <std::string, double> params = variable_value_mapping(splitted_production_rule[0], module);
                    if(condition_check(splitted_production_rule[1], params)){
                        result += expand(module, splitted_production_rule[2], params);
                    }
                }
            }
        }
    }
    return result;
}

// Expand the axiom using the production rules to generate the string that the turtle will parse
std::string L_System::generateWord(const int steps) const{
    std::string word = axioms;
    for (int i = 0; i<steps; i++){
        word = next_step(word);
    }
    return word;
}




