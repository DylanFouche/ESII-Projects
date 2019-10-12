#D Fouche
#UCT CS3
#FCHDYL001

CC = g++
FLAGS = -std=c++11 -Wall -lm -lrt -lwiringPi -lpthread -L blynk-library/src -L blynk-library/linux -L blynk-library/src/Blynk -L blynk-library/src/utility
INC = -I include -I src -I blynk-library/linux -I blynk-library/src -I blynk-library/src/Blynk -I blynk-library/src/utility

SRCDIR = src
TESTDIR = test
BUILDDIR = build
BINDIR = bin

#the name of the file where we have our main method
DRIVER = proja
#the name of our target executable
EXE = driver

SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
TESTOBJECTS = $(filter-out $(BUILDDIR)/$(DRIVER).o,$(OBJECTS))

#rule for linking
$(EXE): $(OBJECTS)
	$(CC)  $^ $(FLAGS) -o $(BINDIR)/$(EXE)

#rule for compiling
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(FLAGS) $(INC) -c $< -o $@

#delete all object files
clean:
	rm -f $(BUILDDIR)/*.o

#deletes all object files and executables
nuke:
	rm -f $(BINDIR)/$(EXE)
	rm -f $(BUILDDIR)/*.o

#run the driver executable
run:
	./$(BINDIR)/$(EXE) $(ARGV)
