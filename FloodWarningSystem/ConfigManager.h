#ifndef ConfigManager_h
#define ConfigManager_h

#define SAMPLE_DEFINE 421412

class ConfigManager{
    // private json config;
    // public string getConfigVar(string attr);
    // public string getConfigVar(string parent, string attr);
    // public void updateConfigVar(string attr, string val);
    // public void updateConfigVar(string parent, string attr, string val);
    public:
        ConfigManager();
        static String getConfigVar(String attr);
    

};

#endif