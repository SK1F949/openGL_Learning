#include "filereader.h"
#include <algorithm>
#include <cctype>

bool Filereader::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    config_map.clear();
    std::string line;

    while (std::getline(file, line))
    {
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
        {
            line = line.substr(0, comment_pos);
        }

        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty())
            continue;

        size_t delimiter_pos = line.find('=');
        if (delimiter_pos != std::string::npos)
        {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);

            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (!key.empty())
            {
                config_map[key] = value;
            }
        }
    }

    file.close();
    return true;
}

bool Filereader::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    for (const auto &pair : config_map)
    {
        file << pair.first << "=" << pair.second << "\n";
    }

    file.close();
    return true;
}

std::string Filereader::getString(const std::string &key, const std::string &default_value) const
{
    auto it = config_map.find(key);
    return (it != config_map.end()) ? it->second : default_value;
}

int Filereader::getInt(const std::string &key, int default_value) const
{
    auto it = config_map.find(key);
    if (it != config_map.end())
    {
        try
        {
            return std::stoi(it->second);
        }
        catch (...)
        {
            return default_value;
        }
    }
    return default_value;
}

float Filereader::getFloat(const std::string &key, float default_value) const
{
    auto it = config_map.find(key);
    if (it != config_map.end())
    {
        try
        {
            return std::stof(it->second);
        }
        catch (...)
        {
            return default_value;
        }
    }
    return default_value;
}

bool Filereader::getBool(const std::string &key, bool default_value) const
{
    auto it = config_map.find(key);
    if (it != config_map.end())
    {
        std::string value = it->second;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if (value == "true" || value == "1" || value == "yes")
            return true;
        if (value == "false" || value == "0" || value == "no")
            return false;
    }
    return default_value;
}

void Filereader::setString(const std::string &key, const std::string &value)
{
    config_map[key] = value;
}

void Filereader::setInt(const std::string &key, int value)
{
    config_map[key] = std::to_string(value);
}

void Filereader::setFloat(const std::string &key, float value)
{
    config_map[key] = std::to_string(value);
}

void Filereader::setBool(const std::string &key, bool value)
{
    config_map[key] = value ? "true" : "false";
}

bool Filereader::contains(const std::string &key) const
{
    return config_map.find(key) != config_map.end();
}

void Filereader::clear()
{
    config_map.clear();
}