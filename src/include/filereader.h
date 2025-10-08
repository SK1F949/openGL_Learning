#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <unordered_map>
#include <fstream>

class Filereader {
private:
    std::unordered_map<std::string, std::string> config_map;
    
public:
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    
    std::string getString(const std::string& key, const std::string& default_value = "") const;
    int getInt(const std::string& key, int default_value = 0) const;
    float getFloat(const std::string& key, float default_value = 0.0f) const;
    bool getBool(const std::string& key, bool default_value = false) const;
    
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setBool(const std::string& key, bool value);
    
    bool contains(const std::string& key) const;
    void clear();
};

#endif