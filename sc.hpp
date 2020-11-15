#ifndef sc_hpp
#define sc_hpp

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
#include <seccomp.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <fstream>
#include <grp.h>

using namespace std;

struct _Config{
	int argc;
	char **argv;
	uid_t uid;
	gid_t gid;
	int rfd, wfd;
	int fd_to_close[2];
	char *mount_dir;
};

typedef struct _Config *Config;

#define STACK_SIZE (8*1024*1024)
#define NEWNS_OFFSET (10000)
#define NEWNS_LENGTH (1000)

int child(void *arg);
int capabilities();
int syscalls();
int parentSetUidGidMap(pid_t cpid, int rfd, int wfd);
int childWaitForMap(int rfd, int wfd);
int setUidGid(uid_t uid, gid_t gid);

#endif