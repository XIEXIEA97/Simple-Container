#include "sc.hpp"

int main(int argc, char *argv[]){
	int rc = -1;
	Config config = new struct _Config;
	int pfd[2]; // for child read and parent write 
	int cfd[2]; // for parent read and child write
	char *stack = new char[STACK_SIZE];
	int flags = CLONE_NEWNS
			| CLONE_NEWCGROUP
			| CLONE_NEWPID
			| CLONE_NEWIPC
			| CLONE_NEWNET
			| CLONE_NEWUTS
			| SIGCHLD;		// so the parent can wait
	pid_t cpid;

	config->argc = argc - 1;
	config->argv = &argv[argc - config->argc];
	config->uid = config->gid = 0;
	// fprintf(stdout, "\n");
	if(rc = pipe(pfd)){
		fprintf(stderr, "pipe failed: %s\n", strerror(errno));
		goto out;
	}
	config->rfd = pfd[0];
	config->fd_to_close[0] = pfd[1];
	// set other options
	if(rc = pipe(cfd)){
		fprintf(stderr, "pipe failed: %s\n", strerror(errno));
		goto out;
	}
	config->wfd = cfd[1];
	config->fd_to_close[1] = cfd[0];

	if(!stack){
		fprintf(stderr, "stack allocation failed: %s\n", strerror(errno));
		rc = -1;
		goto out;
	}

	if((cpid = clone(child, stack + STACK_SIZE, flags, config)) == -1){
		fprintf(stderr, "clone failed: %s\n", strerror(errno));
		rc = -1;
		goto out;
	}

	// cout << "parent pid: " << getpid() << endl << "child pid: " << cpid << endl;
	if(rc = parentSetUidGidMap(cpid, cfd[0], pfd[1])){
		fprintf(stderr, "parentSetUidGidMap failed: %s\n", strerror(errno));
		goto out;
	}

	wait(NULL);

out:
	if(cpid) kill(cpid, SIGKILL);
	for(int i=0; i<2; i++){
		if(pfd[i]) close(pfd[i]);
		if(cfd[i]) close(cfd[i]);
	}
	if(stack) delete(stack);
	if(config) delete(config);

	return rc;
}