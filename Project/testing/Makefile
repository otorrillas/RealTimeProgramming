# Which compiler to use
CXX = g++

# Compiler flags go here.
CXXFLAGS = -g -std=c++11 
#-Wall -Wextra

# Linker flags go here.
LDFLAGS = -lcomedi -lm

# list of sources
DRIVERSRC = ElevInterface.cpp PanelInterface.cpp io.cpp
COMMSRC = Communication.cpp Communication_Lift.cpp Communication_Master.cpp
CONTSRC = ClientControl.cpp

CLIENTSRC = ClientMain.cpp
MASTERSRC = MasterMain.cpp

CLISTENERSRC = ClientListener.cpp
MLISTENERSRC = MasterListener.cpp

CLIENTTESTSRC = ClientTester.cpp

# program executable file name.

MASTER = MasterMain
CLIENT = ClientMain
CLIENTTEST = ClientTest

CLISTENER = ClientListener
MLISTENER = MasterListener

# top-level rule, to compile everything.
all: $(MASTER) $(CLIENT) $(CLISTENER) $(MLISTENER) $(CLIENTTEST)

# Define all object files.
DRIVEROBJ = $(DRIVERSRC:.cpp=.o)
COMMOBJ = $(COMMSRC:.cpp=.o)
CONTOBJ = $(CONTSRC:.cpp=.o)
MASTEROBJ = $(MASTERSRC:.cpp=.o)
CLIENTOBJ = $(CLIENTSRC:.cpp=.o)
CLISTENEROBJ = $(CLISTENERSRC:.cpp=.o)
MLISTENEROBJ = $(MLISTENERSRC:.cpp=.o)
CLIENTTESTOBJ = $(CLIENTTESTSRC:.cpp=.o)

# rule to link the program
$(CLIENT): $(DRIVEROBJ) $(COMMOBJ) $(CONTOBJ) $(CLIENTOBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENTTEST): $(COMMOBJ) $(CLIENTTESTOBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)



$(MASTER): $(DRIVEROBJ) $(COMMOBJ) $(MASTEROBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLISTENER): $(COMMOBJ) $(CLISTENEROBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(MLISTENER): $(COMMOBJ) $(MLISTENEROBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C++ source files.
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 


# rule for cleaning re-compilable files.
clean:
	rm -f $(MASTEROBJ) $(CLIENTOBJ) $(DRIVEROBJ) $(COMMOBJ) $(CONTOBJ) $(CLISTENEROBJ) $(MLISTENEROBJ)

rebuild:	clean all

.PHONY: all rebuild clean