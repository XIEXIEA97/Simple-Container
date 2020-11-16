#include "sc.hpp"

int mounts(){
  // mount root fs
  fprintf(stdout, "Mounting new root fs\n");
  int mount_rc;
  if (mount_rc = chroot("/contained")) {
		return mount_rc;
	}
  // if (mount_rc = chdir("/")) {
	// 	return mount_rc;
	// }
  return 0;
}
