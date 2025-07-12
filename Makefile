# Compiler to use
CXX = g++

# Compiler flags:
# -std=c++17: Use C++17 standard
# -Wall: Enable all standard warnings
# -Iinclude: Add the 'include' directory to the include path
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Linker flags (none needed for this simple project)
LDFLAGS =

# Source files
SRCS = src/main.cpp src/Account.cpp src/Transaction.cpp src/UserAuth.cpp src/Utility.cpp

# Object files (generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Executable target name
TARGET = bank_management_system

# Default target: builds the executable
all: $(TARGET)

# Rule to build the executable from object files
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile each .cpp file into a .o file
# $<: the first prerequisite (e.g., src/Account.cpp)
# $@: the target (e.g., src/Account.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule: removes object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
	# Optionally remove data files for a clean slate
	# rm -f data/accounts.dat data/logs.txt

.PHONY: all clean
