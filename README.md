# Simple-Container
build the container binary use:
```sh
$ make
```
build some test binaries:
```sh
$ make testc & make testk
```
call the container and run the binary echo in the repo which echo the command line arguments, make sure the binary is executable for all users:
```sh
$  ./sc /echo a b c 1 2 3
```
run a shell in the container
```sh
$ ./sc /bin/sh
```
run a test shell script to see something different inside the container, make sure the script is executable for all users:
```sh
# /test.sh
```
run a binary to test mknod system call, and show current capabilities inside the container using cap_get_proc(). these system calls should be banned by dropping capabilities. make sure the binary is executable for all users:
```sh
# /test_cap
```
run a binary to test keyctl system call which should be banned specificlly. make sure the binary is executable for all users:
```sh
# /test_keyctl
```
to see current uid and gid inside the container, which should all be 0 (as hardcoded):
```sh
# id
```