#Directory
INCDIR = include
SRCDIR = src
OBJDIR = obj


CXX       = /usr/bin/g++
#flags for the actual compilation
CXXFLAGS  = -fPIC -isystem$(shell root-config --incdir) -std=c++11 -g -fopenmp -ggdb -Wall -Wextra  -Wconversion -Wshadow -I/usr/include -I$(INCDIR)
#flags for the linker
LDFLAGS   = $(shell root-config --libs --glibs) -lMathMore  -lm -fopenmp
#flags used for the linking of the shared object library
SOFLAGS   = -fPIC -shared


#user definitions concerning the project
PROJ      = AutoNRF
_OBJ      = DataReader.o Functions.o Efficiency.o Flux.o Gamma.o Output.o
OBJ       = $(_OBJ:%=$(OBJDIR)/%)
MAIN      = $(OBJDIR)/main.o


HDR     = $(_OBJ:%.o=$(INCDIR)/%.h)
DHDR    = $(INCDIR)/$(PROJ)LinkDef.h
MAINCPP = $(MAIN:$(OBJDIR)/%.o=$(SRCDIR)/%.cpp)
SANITIZE = -fsanitize=address -fsanitize=undefined


INPUT = makeInputFile


all: $(INPUT) $(PROJ)

$(PROJ): $(MAIN) lib$(PROJ).so 
	$(CXX) -o $@ $^ $(LDFLAGS) -L. -l$(PROJ)

lib$(PROJ).so: $(OBJ) G__$(PROJ).o
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@

$(MAIN): $(MAINCPP) $(HDR)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)  

$(INPUT):
	$(CXX) -o $(INPUT) $(CXXFLAGS) src/$(INPUT).cpp

# root dictionary generation
G__$(PROJ).cpp: $(HDR) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CXXFLAGS) $^

sanitize: clean sanitizeflags $(PROJ)

sanitizeflags:
	$(eval CXXFLAGS=$(CXXFLAGS) $(SANITIZE))
	$(eval LDFLAGS=$(LDFLAGS) $(SANITIZE))
	
new:	clean all
	
val: $(PROJ)
	valgrind --log-file="val.log" --suppressions=$(ROOTSYS)/etc/valgrind-root.supp --tool=helgrind --suppressions=$(ROOTSYS)/etc/helgrind-root.supp ./AutoNRF -i input.par 
	
val4: $(PROJ)
	valgrind --log-file="val.log" --suppressions=$(ROOTSYS)/etc/valgrind-root.supp --tool=helgrind --suppressions=$(ROOTSYS)/etc/helgrind-root.supp ./AutoNRF -i input.par -t 4
	
valvis: $(PROJ)
	valgrind --tool=massif --tool=helgrind ./AutoNRF -i input.par -t 4 


	
run: $(PROJ)
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./$(PROJ) -i input.par -t 4
#$$ for bash Enviroment Variable

clear:
	@rm -rf $(PROJ) $(INPUT) $(OBJDIR)/* G__* Output/ ; echo Make the Project clean again!

clean: clear