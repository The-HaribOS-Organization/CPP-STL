LDFILE = parser.ld 
CXX = clang
LD = ld

LDS = parser.ld

CXXFLAGS = -target x86_64-unknown-windows -nostdlib -ffreestanding -fshort-wchar -mno-red-zone
INCLUDE_HEADERS = -I include

# Files
SRCS := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) 
OBJ_DIR := obj
OBJS := $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Linker
LDFLAGS = -T $(LDFILE) -static -Bsymbolic -nostdlib

all: $(OBJS) build

$(OBJ_DIR)/%.o: src/%.cpp
	@ mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE_HEADERS) -c $< -o $@

build:
	$(LD) $(LDFLAGS) -o bin/parser.elf $(OBJS)

setup:
	@ mkdir -p obj
	@ mkdir -p bin

clean:
	rm -rf $(OBJ_DIR)
	rm -f bin/parser.elf

.PHONY: all build setup clean
