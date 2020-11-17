#include "sc.hpp"
#include <dirent.h>
#include <sys/mount.h>

int mounts(){
  // mount root fs
  fprintf(stdout, "Mounting new root fs\n");
  int mount_rc;
  // if (mount_rc = mount("proc", "proc", "/proc", 0, "")){
  //   fprintf(stdout, "MOUNT FAILED\n");
  //   return mount_rc;
  // }

  if (mount_rc = chroot("rootfs")) {
    fprintf(stdout, "CHROOT FAILED\n");
		return mount_rc;
	}

  if (mount_rc = chdir("/")) {
    fprintf(stdout, "CHDIR FAILED\n");
		return mount_rc;
	}

  // DIR *d;
  // struct dirent *dir;
  // d = opendir("/bin");
  // if (d)
  // {
  //     while ((dir = readdir(d)) != NULL)
  //     {
  //         fprintf(stdout, "%s\n", dir->d_name);
  //     }
  //     closedir(d);
  // }

  return 0;
}
