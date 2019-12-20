OBJ_FILES = intersection.o road.o signal.o vehicle.o traffic.o
BIN_NAME = sim
CC_OPTIONS = -g -Wall -ggdb3 -I.

.SUFFIXES: .cpp .o
.cpp.o:
	g++ -c $< $(CC_OPTIONS)

$(BIN_NAME): $(OBJ_FILES)
	g++ $(OBJ_FILES) -o $@ $< $(CC_OPTIONS)

.PHONY: clean
clean:
	rm -f *.o $(BIN_NAME)