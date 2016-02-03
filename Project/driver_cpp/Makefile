# Which compiler to use
CXX = g++

# Compiler flags go here.
CXXFLAGS = -g -std=c++11 -Wall -Wextra

# Linker flags go here.
LDFLAGS = -lcomedi -lm

# list of sources
ELEVSRC = ElevInterface.cpp PanelInterface.cpp io.cpp main.cpp

# program executable file name.
TARGET = elevator

# top-level rule, to compile everything.
all: $(TARGET) 

# Define all object files.
ELEVOBJ = $(ELEVSRC:.cpp=.o)

# rule to link the program
$(TARGET): $(ELEVOBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C++ source files.
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJ)

rebuild:	clean all

.PHONY: all rebuild clean