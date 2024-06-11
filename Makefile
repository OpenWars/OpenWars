RM			= rm -f
MKDIR		= mkdir -p

CXX			= g++
CXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS		= `pkg-config raylib --cflags --libs`

CPPCHECK	= cppcheck
CLANGXX		= clang++

all: openwars

clean:
	$(RM) ./out/openwars

check:
	$(CPPCHECK) --language=c++ --std=c++17 ./src/main.cpp
	$(CLANGXX) --analyze -Xclang -analyzer-output=html $(CXXFLAGS) -Isrc \
		-o ./out/analysis \
		./src/main.cpp \
		$(LDFLAGS)

openwars:
	$(MKDIR) ./out/
	$(CXX) $(CXXFLAGS) -o ./out/openwars ./src/main.cpp $(LDFLAGS)
