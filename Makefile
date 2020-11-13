all: sc.hpp main.cpp child.cpp capabilities.cpp syscalls.cpp
	gcc -o sc main.cpp child.cpp capabilities.cpp syscalls.cpp -lstdc++ -lcap -lseccomp

test: sc.hpp test.cpp
	gcc -o test test.cpp -lstdc++ -lcap -lseccomp -lkeyutils

testk: test_keyctl.c
	gcc -o test_keyctl test_keyctl.c -lkeyutils