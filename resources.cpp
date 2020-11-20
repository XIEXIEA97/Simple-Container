#include "sc.hpp"
#include <linux/limits.h>
#include <vector>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

using namespace std;

using Setting = pair<string, string>;
using CGRP_Control = pair<string, vector<Setting>>;


int write_setting(string dir, string name, string value) {
    char path[PATH_MAX] = {0};
    int fd = 0;
    fprintf(stderr, "setting %s: %s\n", name.c_str(), value.c_str());
    if (snprintf(path, sizeof(path), "%s/%s", dir.c_str(),
                name.c_str()) == -1) {
        fprintf(stderr, "snprintf failed: %m\n");
        return -1;
    }
    if ((fd = open(path, O_WRONLY)) == -1) {
        fprintf(stderr, "opening %s failed: %m\n", path);
        return -1;
    }
    if (write(fd, value.c_str(), value.length()) == -1) {
        fprintf(stderr, "writing to %s failed: %m\n", path);
        close(fd);
        return -1;
    }
    fprintf(stderr, "finished setting %s: %s", name.c_str(), value.c_str());
    close(fd);
    return 0;
}

int write_control(CGRP_Control cgrp_control, string hostname){
    string control_name = cgrp_control.first;
    char dir[PATH_MAX] = {0};
    fprintf(stderr, "%s\n", control_name.c_str());
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s/%s",
                control_name.c_str(), hostname.c_str()) == -1) {
        return -1;
    }
    fprintf(stderr, "Setting %s\n", dir);
    if (mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
        fprintf(stderr, "mkdir %s failed: %m\n", dir);
        return -1;
	}
    for(auto setting : cgrp_control.second){
        if(write_setting(dir, setting.first, setting.second) == -1){
            fprintf(stderr, "Write setting failed\n");
            return -1;
        }
    }
    return 0;
}

int resources(string hostname){
    fprintf(stderr, "setting cgroups...\n");

    // init
    vector<CGRP_Control> controls;

    // Memory
    vector<Setting> memory_settings;
    memory_settings.push_back({"memory.limit_in_bytes", "100000000"});
    memory_settings.push_back({"memory.kmem.limit_in_bytes", "100000000"});
    memory_settings.push_back({"memory.swappiness", "0"});
    char mypid[6];
    int pid = getpid();
    sprintf(mypid, "%d", pid);
    memory_settings.push_back({"tasks", mypid});
    controls.push_back({"memory", memory_settings});

    for(CGRP_Control control : controls){
        if(write_control(control, hostname)){
            return -1;
        }
    }
    fprintf(stderr, "done\n");
    return 0;
}