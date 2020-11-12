#include <errno.h>
#include <string.h>
#include <sys/prctl.h>
#include <cstdio>
#include <sys/types.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include <iostream>
#include <sys/wait.h>

using namespace std;

struct _Config{
	int argc;
	char **argv;
	uid_t uid;
	int fd;
	char *hostname;
	char *mount_dir;
};

typedef struct _Config *Config;

#define STACK_SIZE (1024*1024)

int child(void *arg);
int capabilities();
