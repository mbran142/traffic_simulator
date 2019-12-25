OBJ_FILES =  road.o signal.o vehicle.o traffic.o intersection.o
BIN_NAME = sim
CC_OPTIONS = -g -Wall -ggdb3 -I.

$(BIN_NAME): $(OBJ_FILES)
	g++ -o $@ $^ $(CC_OPTIONS)

%.o : %.cpp %.h traffic.h
	g++ -c -o $@ $< $(CC_OPTIONS)

.PHONY: clean
clean:
	rm -f *.o $(BIN_NAME)