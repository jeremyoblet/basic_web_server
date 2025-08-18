cmake_minimum_required(VERSION 3.28)
project(cherokee)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(cherokee
        src/main.cpp
        src/ClientSocket.cpp
        src/HttpData.cpp
        src/HttpServer.cpp
        src/Utils.cpp
        src/HttpHeadHandler.cpp
        src/HttpGetHandler.cpp
        src/HttpDeleteHandler.cpp
        src/HttpPostHandler.cpp
        src/HttpPutHandler.cpp
        src/HttpCGIHandler.cpp
        src/HttpRequestHandler.cpp
        src/Config.cpp
        src/CacheManager.cpp)

# Support for Winsock2
if(WIN32)
    target_link_libraries(cherokee wsock32 ws2_32)
endif()