#include <Config.h>
#include "HttpServer.h"

#include <iostream>


int main() {
    try {
        Config& config = Config::GetInstance();
        if (!config.LoadConfig("config.txt")) {
            std::cerr << "Error: Could not load config file" << std::endl;
            return 1;
        }

        HttpServer server(config.GetPort(), 4); // Using 4 threads in the thread pool
        server.start();

    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}