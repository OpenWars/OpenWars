CXX = g++
CXXFLAGS = -Wall -O0 -lraylib

all: openwars

clean:
	$(RM) ./out/openwars

openwars:
	$(CXX) -o ./out/openwars ./src/main.cpp $(CXXFLAGS)
