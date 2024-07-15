CXX = g++
CXXFLAGS = -pedantic-errors -Wall -Wextra -Werror -O3 -g

TARGET = csp

all: $(TARGET)
debug: $(TARGET)-debug

$(TARGET): restriction.o csp.o backtracking.o main.o
	$(CXX) -o $(TARGET) $(CXXFLAGS) restriction.o csp.o backtracking.o main.o

$(TARGET)-debug: restriction.o csp-debug.o backtracking-debug.o main-debug.o
	$(CXX) -o $(TARGET) $(CXXFLAGS) -DDEBUG restriction.o csp.o backtracking.o main.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

main-debug.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -DDEBUG -c src/main.cpp

backtracking.o: src/backtracking.hpp src/backtracking.cpp
	$(CXX) $(CXXFLAGS) -c src/backtracking.cpp

backtracking-debug.o: src/backtracking.hpp src/backtracking.cpp
	$(CXX) $(CXXFLAGS) -DDEBUG -c src/backtracking.cpp

csp.o: src/csp.hpp src/csp.cpp
	$(CXX) $(CXXFLAGS) -c src/csp.cpp

csp-debug.o: src/csp.hpp src/csp.cpp
	$(CXX) $(CXXFLAGS) -DDEBUG -c src/csp.cpp

restriction.o: src/restriction.hpp src/restriction.cpp
	$(CXX) $(CXXFLAGS) -c src/restriction.cpp

clean:
	rm -f $(TARGET) *.o
