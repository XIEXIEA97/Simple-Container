#include "sc.hpp"

int parentSetUidGidMap(pid_t cpid, int rfd, int wfd){
	bool child_ready, parent_ready = true;
	// read for a ready signal, so the map is set after child is created
	if(read(rfd, &child_ready, sizeof(bool)) != sizeof(bool)){
		fprintf(stderr, "setUidGidMap read failed: %s\n", strerror(errno));
		return -1;
	}

	fprintf(stdout, "parent start to set uid/gid map\n");

	// /proc/[pid]/uid_map and /proc/[pid]/gid_map
	string uid_path = string("/proc/") + to_string(cpid) + string("/uid_map"),
	gid_path = string("/proc/") + to_string(cpid) + string("/gid_map");
	ofstream uid_map(uid_path), gid_map(gid_path);
	// The first two numbers specify the starting user ID in each of
    // the two user namespaces.  The third number specifies the length of
    // the mapped range.
	if(child_ready && uid_map.is_open() && gid_map.is_open()){
		string map_line = string("0 ") + to_string(NEWNS_OFFSET) + string(" ") + to_string(NEWNS_LENGTH) + string("\n");
		uid_map << map_line;
		gid_map << map_line;

		uid_map.close();
		gid_map.close();
	} else{
		cout << uid_path << endl << gid_path << endl;
		fprintf(stderr, "parent write to map file failed: %s\n", strerror(errno));
		return -1;
	}

	if(write(wfd, &parent_ready, sizeof(bool)) != sizeof(bool)){
		fprintf(stderr, "setUidGidMap write failed: %s\n", strerror(errno));
		return -1;
	}

	fprintf(stdout, "parent set uid/gid map complete\n");

	return 0;
}

int childWaitForMap(int rfd, int wfd){
	// set new namespace for the process
	bool child_ready = !(bool)unshare(CLONE_NEWUSER), parent_ready;
	if(!child_ready){
		fprintf(stderr, "unshare failed: %s\n", strerror(errno));
		return -1;
	}
	if(write(wfd, &child_ready, sizeof(bool)) != sizeof(bool)){
		fprintf(stderr, "childWaitForMap write failed: %s\n", strerror(errno));
		return -1;
	}
	if(read(rfd, &parent_ready, sizeof(bool)) != sizeof(bool)){
		fprintf(stderr, "childWaitForMap read failed: %s\n", strerror(errno));
		return -1;
	}
	assert(parent_ready);
	return 0;
}

int setUidGid(uid_t uid, gid_t gid){
	fprintf(stdout, "child start to set uid/gid\n");

	if(setresuid(uid, uid, uid)){ 
		fprintf(stderr, "setresuid failed: %s\n", strerror(errno));
		return -1;
	}

	if(setresgid(gid, gid, gid)){ 
		fprintf(stderr, "setresgid failed: %s\n", strerror(errno));
		return -1;
	}

	if(setgroups(1, &gid)){ 
		fprintf(stderr, "setgroups failed: %s\n", strerror(errno));
		return -1;
	}

	fprintf(stdout, "child uid/gid set complete\n");
	return 0;
}