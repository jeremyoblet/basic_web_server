# Variables
CXX := g++
CXXFLAGS := -Iinclude -Wall -Wextra -std=c++17
SRCDIR := src
INCDIR := include
BUILDDIR := build
TARGET := cherokee

# Find all .cpp files in the src directory
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
# Generate object files paths
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Rule to build object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean rule
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Phony target to indicate which targets are not real files
.PHONY: all
all: $(TARGET)