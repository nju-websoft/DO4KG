CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++11

all : bin bin/construct

bin :
	mkdir bin

bin/construct : construct.cpp pruned_highway_labeling.h
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $^

clean :
	rm -rf bin

.PHONY : all clean
