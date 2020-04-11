SHELL = /bin/bash
.SUFFIXES:
.SUFFIXES: .cpp .h .o

# project set up and directories
CXX	    = g++
INCLDIR	= inc/
INCLDIRSFML	= SFML-2.5.1/include/
BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/
LIBPATH = /home/hubert/repos/Snake_game/SFML-2.5.1/lib

# final executable name
_BIN = snakegame
BIN	 = $(addprefix $(BINDIR), $(_BIN))

# files; here all object files will be stored in $(OBJDIR), with the
# same base names as corresponding c files from SRCDIR
SRC	  = $(wildcard src/*.cpp)
_OBJS = $(patsubst src/%.cpp, %.o, $(SRC))
OBJS  = $(addprefix $(OBJDIR), $(_OBJS))
LIBS  = -lsfml-system -lsfml-window -lsfml-graphics

# compilation flags
CXXFLAGS = -Wall -std=c++14 -pedantic -g -I$(INCLDIR) -I$(INCLDIRSFML)

# compile binary and object files
.PHONY: all
all: $(BIN)

$(BIN): $(BINDIR) $(OBJS)
	$(CXX) $(OBJS) -o $(BIN) -L/$(LIBPATH) $(LIBS)
	@echo snakegame built in bin/snakegame

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJS): $(OBJDIR) $(SRC) $(INCLDIR)/*
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# clean entire project directory
.PHONY: clean
clean:
	- rm -rf $(BINDIR) $(OBJDIR)


# check code quality
.PHONY: cppcheck memcheck
cppcheck:
	cppcheck --enable=all --language=c++ --std=c++14 --inconclusive \
	--suppress=missingInclude $(SRC) -i $(INCLDIR) -i $(INCLDIRSFML)

memcheck: all
	valgrind -v --show-leak-kinds=all --leak-check=full --track-origins=yes \
	./$(BIN)

# debugging make
print-% :
	@echo $* = $($*)