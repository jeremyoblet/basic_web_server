#include "Config.h"

#include <fstream>
#include <sstream>


bool Config::LoadConfig(const std::string& config_file)
{
    std::ifstream file(config_file);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string token_string;
        std::string value_string;

        token_string = line.substr(0, line.find('='));
        value_string = line.substr(line.find('=') + 1);

        if (token_string == "RootDir") {
            RootDir = value_string;
        }

        if (token_string == "TimeoutDelay") {
            TimeoutDelay = std::stof(value_string);
        }

        if(token_string == "Port") {
            Port = std::stoi(value_string);
        }
    }
    return true;
}

std::string Config::GetRootDir() const {
    return RootDir;
}

float Config::GetTimeoutDelay() const {
    return TimeoutDelay;
}

int Config::GetPort() const {
    return Port;
}

