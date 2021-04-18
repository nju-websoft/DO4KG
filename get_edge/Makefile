CXX = g++
CXXFLAGS = -O3 -std=c++11


all: get_edge

get_graph: get_edge.cpp
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $^

clean:
	rm -r get_edge
	
