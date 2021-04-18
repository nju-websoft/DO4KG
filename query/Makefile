CXX = g++
CXXFLAGS = -O2 -std=c++11


all: query

get_graph: query.cpp
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $^

clean:
	rm -r query
	
