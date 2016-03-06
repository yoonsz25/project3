CXX=g++
CXXFLAGS=-g -std=c++11 #-Wall
BINARIES=prog3

test:${BINARIES}
	./prog3
	
prog3: BTree.o BTreeNode.o AdList.o Main.o 
	$(CXX) $(CXXFLAGS) $^ -o $@

	
clean:
	/bin/rm -f ${BINARIES} *.o