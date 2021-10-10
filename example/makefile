CXX=g++
CFLAGS= -std=c++17 -D LOCALTEST -I../include -I../include/core -g
#CFLAGS= -g -std=c++17 -D LOCALTEST

SRC=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp,%.out,$(SRC))

all: $(TARGET)
	@echo done

%.out : %.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)
