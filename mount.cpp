#include "sc.hpp"

int mounts(){
  // mount root fs
  fprintf(stdout, "Mounting new root fs\n");
  int mount_rc;
  if (mount_rc = pivot_root("/contained", "/tmp/old_root")) {
		return mount_rc;
	}
  // if (mount_rc = chdir("/")) {
	// 	return mount_rc;
	// }
  return 0;
}
