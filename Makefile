CXX = g++
CXXFLAGS = -O2

matmul: main.o matmul.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp matmul.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

matmul.o: matmul.cpp matmul.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f matmul *.o

.PHONY: clean
