cmake_minimum_required(VERSION 3.20)
project(cherokee LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  src/CacheManager.cpp
)

target_include_directories(cherokee PRIVATE include)

if(WIN32)
  target_link_libraries(cherokee PRIVATE ws2_32)
else()
  find_package(Threads REQUIRED)
  target_link_libraries(cherokee PRIVATE Threads::Threads)
endif()
