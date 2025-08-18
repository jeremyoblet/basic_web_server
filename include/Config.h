#pragma once

#include <string>


class Config
{
public:
    // Deleted copy constructor and assignment operator to prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    static Config& GetInstance() {
        static Config instance;
        return instance;
    }

    bool LoadConfig(const std::string& config_file);

    std::string GetRootDir() const;
    float GetTimeoutDelay() const;
    int GetPort() const;

private:
    Config() = default;
    ~Config() = default;

private:
    std::string RootDir    = "./root";
    float        TimeoutDelay = 5.0f;
    int          Port         = 8080;
};