CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++11

all : bin bin/construct

bin :
	mkdir bin

bin/construct : main.cpp
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $^

clean :
	rm -rf bin

.PHONY : all clean
