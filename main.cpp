#include "sc.hpp"

int main(int argc, char *argv[]){
	Config config = new struct _Config;
	config->argc = argc - 1;
	config->argv = &argv[argc - config->argc];
	// fprintf(stdout, "\n");

	char *stack = new char[STACK_SIZE];
	if(!stack){
		fprintf(stderr, "stack allocation failed: %s\n", strerror(errno));
		return 1;
	}

	int flags = CLONE_NEWNS
			| CLONE_NEWCGROUP
			| CLONE_NEWPID
			| CLONE_NEWIPC
			| CLONE_NEWNET
			| CLONE_NEWUTS
			| SIGCHLD;

	int pid_child, status;
	if(pid_child = clone(child, stack + STACK_SIZE, flags, config)){
		fprintf(stderr, "clone failed: %s\n", strerror(errno));
		return 1;
	}
	
	return 0;
}