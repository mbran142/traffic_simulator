OBJ_FILES = intersection.o road.o signal.o vehicle.o traffic.o
BIN_NAME = sim
CC_OPTIONS = -g -Wall -ggdb3 -I.

%.o : %.cpp %.h
	g++ -c -o $@ $< $(CC_OPTIONS)

$(BIN_NAME): $(OBJ_FILES) traffic.h
	g++ -o $@ $^ $(CC_OPTIONS)

.PHONY: clean
clean:
	rm -f *.o $(BIN_NAME)