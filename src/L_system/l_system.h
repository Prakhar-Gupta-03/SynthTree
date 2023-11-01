#include <vector>
#include <unordered_map>
#include <string> 

class L_System{

private:
    std::vector<std::string> axioms;
    std::vector<std::string> alphabets;
    std::unordered_map<std::string, std::string> production_rules;
    
};