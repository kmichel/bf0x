CXX = g++
CXXFLAGS = -std=c++0x -pedantic -fstrict-aliasing
CXXFLAGS += -fshow-column -fdiagnostics-show-option 
CXXFLAGS += -Wextra -Wall -Weffc++ \
    -Wcast-align -Wcast-qual -Wold-style-cast \
    -Wconversion -Wsign-conversion -Wsign-promo \
    -Wshadow -Wuninitialized -Winit-self \
    -Wwrite-strings -Wdisabled-optimization -Wlogical-op \
    -Wmissing-include-dirs -Wredundant-decls -Wmissing-declarations
CXXFLAGS += -Os -march=native
CXXFLAGS += -finline-functions -finline-small-functions -findirect-inlining

main : main.cpp

clean :
	rm main
