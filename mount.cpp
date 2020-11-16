#include "sc.hpp"
#include <dirent.h>

int mounts(){
  // mount root fs
  fprintf(stdout, "Mounting new root fs\n");
  int mount_rc;
  if (mount_rc = chroot("/contained")) {
		return mount_rc;
	}
  if (mount_rc = chdir("/")) {
		return mount_rc;
	}

  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d)
  {
      while ((dir = readdir(d)) != NULL)
      {
          fprintf(stdout, "%s\n", dir->d_name);
      }
      closedir(d);
  }

  return 0;
}
