CXX = g++
CXXFLAGS = -O3 -std=c++11


all: bin bin/construct_index

bin:
	mkdir -p bin

bin/construct_index: main.cpp
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $^

clean:
	rm -rf bin
	
