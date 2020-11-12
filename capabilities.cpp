#include "sc.hpp"

static int drop[] = {
	CAP_AUDIT_CONTROL,
	CAP_AUDIT_READ,
	CAP_AUDIT_WRITE,
	CAP_BLOCK_SUSPEND,
	// CAP_CHECKPOINT_RESTORE,
	CAP_DAC_READ_SEARCH,
	CAP_FSETID,
	CAP_IPC_LOCK,
	CAP_MAC_ADMIN,
	CAP_MAC_OVERRIDE,
	CAP_MKNOD,
	CAP_SETFCAP,
	CAP_SYSLOG,
	CAP_SYS_ADMIN,
	CAP_SYS_BOOT,
	CAP_SYS_MODULE,
	CAP_SYS_NICE,
	CAP_SYS_RAWIO,
	CAP_SYS_RESOURCE,
	CAP_SYS_TIME,
	CAP_WAKE_ALARM
};

int capabilities(){
	fprintf(stdout, "drop capabilities from bounding set\n");

	int ncap = sizeof(drop) / sizeof(int);
	for(int i=0; i<ncap; i++){
		if(prctl(PR_CAPBSET_DROP, drop[i], 0, 0, 0)){ // drop capabilities from bounding set
			fprintf(stderr, "prctl failed: %s\n", strerror(errno));
			return 1;
		}
	}

	fprintf(stdout, "drop capabilities from inheritable set\n");

	cap_t caps = cap_get_proc(); // get current process's capability
	if(!caps){
		fprintf(stderr, "cap_get_proc failed: %s\n", strerror(errno));
		goto out;
	}

	if(cap_set_flag(caps, CAP_INHERITABLE, ncap, drop, CAP_CLEAR)){
		// drop the capabilities from the caps state
		fprintf(stderr, "cap_set_flag failed: %s\n", strerror(errno));
		goto out;
	}

	if(cap_set_proc(caps)){ // apply caps to set the process's state(inheritable set)
		fprintf(stderr, "cap_set_flag failed: %s\n", strerror(errno));
		goto out;
	}

	fprintf(stdout, "capabilities drop complete\n");

out:
	if(caps)
		cap_free(caps);
	return 0;
}