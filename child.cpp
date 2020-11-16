#include "sc.hpp"


int child(void *arg){
	int rc = -1;
	Config config = (Config)arg;
	string hostname = "simple_container";
	// for(int i=0; i<config->argc; i++){
	// 	cout << config->argv[i] << endl;
	// }

	for(int i=0; i<2; i++)
		close(config->fd_to_close[i]);

	if(rc = sethostname(hostname.c_str(), hostname.size())){
		fprintf(stderr, "sethostname failed: %s\n", strerror(errno));
		goto out;
	}

	// // mount
	if(rc = mounts()){
		fprintf(stderr, "mount change failed: %s\n", strerror(errno));
		goto out;
	}

	if(childWaitForMap(config->rfd, config->wfd)){
		fprintf(stderr, "child wait failed: %s\n", strerror(errno));
		rc = -1;
		goto out;
	}

	if(setUidGid(config->uid, config->gid)){
		fprintf(stderr, "uid/gid set failed: %s\n", strerror(errno));
		rc = -1;
		goto out;
	}

	if(rc = capabilities()){
		fprintf(stderr, "capabilities drop failed: %s\n", strerror(errno));
		goto out;
	}

	if(rc = syscalls()){
		fprintf(stderr, "syscalls block failed: %s\n", strerror(errno));
		goto out;
	}

	fprintf(stdout, "lauch contained environment...\n");
	if(rc = execve(config->argv[0], config->argv, NULL)){
		fprintf(stderr, "contained process execve failed: %s\n", strerror(errno));
		goto out;
	}

out:
	close(config->rfd);
	close(config->wfd);
    return rc;
}
