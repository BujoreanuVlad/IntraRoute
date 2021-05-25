#!/bin/sh
g++ -c -std=c++17 main.cpp
mv main.o bin/
g++ -std=c++17 bin/*.o -o App -lsfml-graphics -lsfml-system -lsfml-window
./App
