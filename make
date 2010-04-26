#!/bin/sh
g++ -std=c++0x -pedantic \
    -fshow-column -fdiagnostics-show-option \
    -fstrict-aliasing \
    -ftemplate-depth-1024 \
    -fvisibility-inlines-hidden -fvisibility=hidden \
    -Wextra -Wall -Weffc++ \
    -Wcast-align -Wcast-qual -Wold-style-cast \
    -Wconversion -Wsign-conversion -Wsign-promo \
    -Wshadow -Wuninitialized -Winit-self \
    -Wwrite-strings -Wdisabled-optimization -Wlogical-op \
    -Wmissing-include-dirs -Wredundant-decls -Wmissing-declarations \
    -Os -march=native \
    -fwhole-program \
    -freorder-blocks \
    -freorder-blocks-and-partition \
    -freorder-functions \
    -fno-exceptions \
    -fno-rtti \
    --combine \
    -DTRACE_ON \
    -finline-functions -findirect-inlining -finline-small-functions \
main.cpp -ggdb -o main && \
cp main main_s && \
strip -s main_s && \
echo "file size: " `ls -lah main | cut -f5 -d\ ` '->' `ls -lah main_s | cut -f5 -d\ `&& \
rm main_s && \
./main
