
//Since Arduino doesnt support c++ STL libraries, a simply used a workaround

#include "ConfigManager.h"

ConfigManager::ConfigManager(){

}

// class ConfigParser{
    // private json config;
    // ConfigParser(string configFile){
    //     std::ifstream file(configFile);
    //     json config = json::parse(file);
    //     this.config = config;
    // }
    // public string getConfigVar(string attr){
    //     return config[attr];
    // }
    // public string getConfigVar(string parent, string attr){
    //     return config[parent][attr];
    // }
    // public void updateConfigVar(string attr, string val){
    //     config[attr] = val;
    // }
    // public void updateConfigVar(string parent, string attr, string val){
    //     config[parent][attr] = val;
    // }
String ConfigManager::getConfigVar(String attr){
    return attr;
}
