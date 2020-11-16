all: sc.hpp main.cpp child.cpp capabilities.cpp syscalls.cpp user_namespace.cpp mount.cpp
	gcc -o sc main.cpp child.cpp capabilities.cpp syscalls.cpp user_namespace.cpp mount.cpp -lstdc++ -lcap -lseccomp

testc: sc.hpp test_cap.cpp
	gcc -o test_cap test_cap.cpp -lstdc++ -lcap -lseccomp -lkeyutils

testk: test_keyctl.c
	gcc -o test_keyctl test_keyctl.c -lkeyutils
