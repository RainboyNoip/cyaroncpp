CXX=g++
CFLAGS="-lboost_system"

SRC=$(wildcard *.hpp)
TARGET=$(patsubst %.cpp,%,$(SRC))

all: $(SRC) main.cpp
	g++ -g -o test main.cpp -I../include -I../include/core
	./test

clean:
	rm -rf $(TARGET)
