CXX=g++
# CXXFLAGS=-std=c++11 -D DEBUG_LOG
CXXFLAGS=-std=c++11 -D DEBUG_LOG
LD_FLAGS=



all: core_test
# all: nba_core nba_tree tvp_test

core_test: core_test.o core.o mb_tree.o buildtvp.o
	$(CXX) -o core_test core_test.o core.o buildtvp.o mb_tree.o $(LD_FLAGS)

core_test.o: core_test.cpp
	$(CXX) -c -g core_test.cpp $(CXXFLAGS)

core.o: core.cpp
	$(CXX) -c -g core.cpp $(CXXFLAGS)

nba_core: nba_core.o mb_tree.o
	$(CXX) -o nba_core nba_core.o mb_tree.o $(LD_FLAGS)

nba_core.o: nba_core.cpp
	$(CXX) -c -g nba_core.cpp $(CXXFLAGS)

tvp_test: buildtvp_test.o buildtvp.o mb_tree.o
	$(CXX) -o tvp_test buildtvp_test.o buildtvp.o mb_tree.o $(LD_FLAGS)

buildtvp_test.o: buildtvp_test.cpp
	$(CXX) -c -g buildtvp_test.cpp $(CXXFLAGS)

buildtvp.o: buildtvp.cpp 
	$(CXX) -c -g buildtvp.cpp $(CXXFLAGS)

nba_tree: build_nba_tree.o mb_tree.o
	$(CXX) -o nba_tree build_nba_tree.o mb_tree.o $(LD_FLAGS)

build_nba_tree.o : build_nba_tree.cpp 
	$(CXX) -c -g build_nba_tree.cpp $(CXXFLAGS)

mb_tree.o: mb_tree.cpp
	$(CXX) -c -g mb_tree.cpp $(CXXFLAGS)

clean:
	rm -f ./*.o ./*.exe* ./nba_tree ./test ./nba_tree ./tvp_test ./nba_core ./core_test