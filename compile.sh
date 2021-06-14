#!/bin/sh
g++ -c -std=c++17 src/*.cpp 
mv *.o bin/
g++ -std=c++17 ./bin/*.o -o App -lsfml-graphics -lsfml-system -lsfml-window
