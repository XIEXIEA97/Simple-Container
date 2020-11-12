all: sc.hpp main.cpp child.cpp capabilities.cpp
	gcc -o sc main.cpp child.cpp capabilities.cpp -lstdc++ -lcap

test: sc.hpp test.cpp
	gcc -o test test.cpp -lstdc++ -lcap