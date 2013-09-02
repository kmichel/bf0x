CXX = clang++
CXXFLAGS = -std=c++11 -pedantic -fstrict-aliasing
CXXFLAGS += -fshow-column -fdiagnostics-show-option 
CXXFLAGS += -Wextra -Wall -Weffc++ \
    -Wcast-align -Wcast-qual -Wold-style-cast \
    -Wconversion -Wsign-conversion -Wsign-promo \
    -Wshadow -Wuninitialized -Winit-self \
    -Wwrite-strings -Wdisabled-optimization \
    -Wmissing-include-dirs -Wredundant-decls
CXXFLAGS += -Os -march=native
CXXFLAGS += -finline-functions

main : main.cpp

clean :
	rm main
