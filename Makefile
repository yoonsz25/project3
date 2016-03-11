CXX=g++
CXXFLAGS=-g -std=c++11 #-Wall
BINARIES=prog3

test:${BINARIES}
	./prog3
	
prog3: BTreeNode.o BTree.o Main.o 
	$(CXX) $(CXXFLAGS) $^ -o $@

	
clean:
	/bin/rm -f ${BINARIES} *.o