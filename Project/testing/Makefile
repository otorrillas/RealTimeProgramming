# Which compiler to use
CXX = g++

# Compiler flags go here.
CXXFLAGS = -g -std=c++11 
#-Wall -Wextra

# Linker flags go here.
LDFLAGS = -lcomedi -lm -pthread 

# list of sources
DRIVERSRC = ElevInterface.cpp PanelInterface.cpp io.cpp
COMMSRC = Communication.cpp Communication_Lift.cpp Communication_Master.cpp
CONTSRC = ClientControl.cpp


# program executable file name.
MASTERSRC = MasterMain.cpp
MASTER = MasterMain

CLIENTSRC = ClientMain.cpp
CLIENT = ClientMain

# top-level rule, to compile everything.
all: $(MASTER) $(CLIENT)

# Define all object files.
DRIVEROBJ = $(DRIVERSRC:.cpp=.o)
COMMOBJ = $(COMMSRC:.cpp=.o)
CONTOBJ = $(CONTSRC:.cpp=.o)
MASTEROBJ = $(MASTERSRC:.cpp=.o)
CLIENTOBJ = $(CLIENTSRC:.cpp=.o)

# rule to link the program
$(CLIENT): $(DRIVEROBJ) $(COMMOBJ) $(CONTOBJ) $(CLIENTOBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(MASTER): $(DRIVEROBJ) $(COMMOBJ) $(MASTEROBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C++ source files.
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 


# rule for cleaning re-compilable files.
clean:
	rm -f $(MASTEROBJ) $(CLIENTOBJ) $(DRIVEROBJ) $(COMMOBJ) $(CONTOBJ)

rebuild:	clean all

.PHONY: all rebuild clean