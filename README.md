# Basic Web Server

A simple HTTP server written in C++ supporting GET, POST, PUT, DELETE, HEAD methods. This project is designed as a learning base or as a lightweight standalone server.

## 📁 Project Structure

- `src/` — Contains the server source files.
- `include/` — Contains the project header files.
- `CMakeLists.txt` / `Makefile` — Build configuration files.
- `cherokee.vcxproj` / `.sln` — Visual Studio project files (Windows).
- `README.md` — This file.

## ⚙️ Main Features

- Support for HTTP methods:
  - GET
  - POST
  - PUT
  - DELETE
  - HEAD
- Cache management.
- Thread pool for client connections.
- Customizable configuration via the `Config` class.

## 🛠️ Build Instructions

### With CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
make
```

### Build on windows with MinGw

```
rm -rf build
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/cherokee.exe
```
