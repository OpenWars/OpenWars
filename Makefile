RM			= rm -f
MKDIR		= mkdir -p

SOURCES		= $(shell find src -type f -iname "*.cpp")
OBJECTS		= $(patsubst src/%.cpp,out/%.o,$(SOURCES))
TARGET		= ./out/openwars

CXX			= g++
CXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic
CXXFLAGS	+= `pkg-config raylib --cflags`
CXXFLAGS	+= `pkg-config lua --cflags`

LD			= $(CXX)
LDFLAGS		= -Isol2/include
LDFLAGS		+= `pkg-config raylib --libs`
LDFLAGS		+= `pkg-config lua --libs`

CPPCHECK	= cppcheck
CLANGXX		= clang++
VALGRIND	= valgrind

ifeq ($(origin DEBUG), environment)
	CXXFLAGS += -Og -g -DOPENWARS_DEBUG
else
	CXXFLAGS += -O2
endif

all: openwars

clean:
	@$(RM) $(TARGET) $(OBJECTS)

check:
	@$(CPPCHECK) --language=c++ --std=c++17 ./src/main.cpp
	@$(CLANGXX) --analyze -Xclang -analyzer-output=html $(CXXFLAGS) \
		-o ./out/analysis \
		./src/main.cpp \
		$(LDFLAGS)

openwars: $(OBJECTS)
	$(LD) -o $(TARGET) $^ $(LDFLAGS)

out/%.o: src/%.cpp | create_dirs
	@$(CXX) $(CXXFLAGS) -c $^ -o $@ $(LDFLAGS)

create_dirs:
	@$(MKDIR) $(sort $(dir $(OBJECTS)))

test:
	$(VALGRIND) \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		$(TARGET)
