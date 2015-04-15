#CC=gcc
#CXX=g++
CC=clang
CXX=clang++

all : leakfinder.so cpp_example

#malloc_hooks.o : malloc_hooks.c
#	$(CC) -g -shared -fPIC malloc_hooks.c
	
leakfinder.so: leakfinder.cpp leakfinder.h
	$(CXX) -DLEAKFINDER_BUILD -fvisibility=hidden -Os -g -pthread -std=c++11 -Wall -rdynamic -shared -fPIC leakfinder.cpp -o leakfinder.so -lpthread -ldl

cpp_example: cpp_example.cpp
	$(CXX) -g -Wall -rdynamic cpp_example.cpp -o cpp_example 

clean:
	rm -rf leakfinder.so cpp_example
