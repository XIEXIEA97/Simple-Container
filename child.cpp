#include "sc.hpp"

int child(void *arg){
	Config config = (Config)arg;
	// for(int i=0; i<config->argc; i++){
	// 	cout << config->argv[i] << endl;
	// }

	if(capabilities()){
		fprintf(stderr, "capabilities drop failed: %s\n", strerror(errno));
		return 1;
	}

	if(execve(config->argv[0], config->argv, NULL)){
		fprintf(stderr, "contained process execve failed: %s\n", strerror(errno));
		return 1;
	}
	wait(NULL);

    return 0;
}