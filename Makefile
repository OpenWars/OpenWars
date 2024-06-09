RM			= rm -f

CXX			= g++
CXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic -O2 -lraylib
LDFLAGS		= `pkg-config raylib --cflags --libs`

all: openwars

clean:
	$(RM) ./out/openwars

openwars:
	$(CXX) $(CXXFLAGS) -o ./out/openwars ./src/main.cpp $(LDFLAGS)
