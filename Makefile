CXX = g++
CXXFLAGS = -Wall -O0 -lraylib

all: compile

clean:
	$(RM) ./out/main

compile:
	$(CXX) $(CXXFLAGS) -o ./out/main ./src/main.cpp -lraylib

test:
	./out/main