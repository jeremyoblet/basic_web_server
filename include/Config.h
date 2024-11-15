#pragma once

#include <string>

#define TOKEN(string) if (token_string == string)

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
    bool IsCgiEnabled() const;
    std::string GetCgiDirectory() const;

private:
    Config() = default;  // Private constructor
    ~Config() = default;

    std::string RootDir;
    float TimeoutDelay;
    int Port;
    bool CgiEnabled;
    std::string CgiDirectory;
};