CXX       = /usr/bin/g++
#flags for the actual compilation
CXXFLAGS  = -fPIC -isystem$(shell root-config --incdir)  -pthread -std=c++11 -g -ggdb -Wall -Wextra -Wconversion -Wshadow -I/usr/include 
#flags for the linker
LDFLAGS   = $(shell root-config --libs) -glibs -lMathMore
#flags used for the linking of the shared object library
SOFLAGS   = -fPIC -shared

#user definitions concerning the project
PROJ    = AutoNRF
OBJ     = DataReader.o Functions.o Efficiency.o Flux.o Output.o 
MAIN    = main.o


HDR     = $(OBJ:%.o=%.h)
DHDR    = $(PROJ)LinkDef.h
MAINCPP = $(MAIN:%.o=%.cpp)
SANITIZE = -fsanitize=address -fsanitize=undefined

$(PROJ): $(MAIN) lib$(PROJ).so
	$(CXX) -o $@ $^ $(LDFLAGS) -L. -l$(PROJ)

lib$(PROJ).so: $(OBJ) G__$(PROJ).o
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@

$(MAIN): $(MAINCPP) $(HDR)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# root dictionary generation
G__$(PROJ).cpp: $(HDR) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CXXFLAGS) $^

sanitize: clean sanitizeflags $(PROJ)

sanitizeflags:
	$(eval CXXFLAGS=$(CXXFLAGS) $(SANITIZE))
	$(eval LDFLAGS=$(LDFLAGS) $(SANITIZE))
	
new:	clean $(PROJ)
	
	
	
	
run: $(PROJ)
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./$(PROJ) -i input.par
#$$ for bash Enviroment Variable
clean:
	@rm -f ${OBJ} G__*  lib$(PROJ).so $(PROJ) *_ACLiC_dict_* *.log ${MAIN}; echo Make the Project clean again!

clear:
	@rm -f ${OBJ} G__*  lib$(PROJ).so $(PROJ) *_ACLiC_dict_* *.log ${MAIN}; echo Make the Project clean again!