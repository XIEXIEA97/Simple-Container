#include "sc.hpp"
#include <dirent.h>
#include <sys/mount.h>
#include <sys/syscall.h>

int pivot_root(const char *new_root, const char *put_old)
{
	return syscall(SYS_pivot_root, new_root, put_old);
}

int mounts(){
  // mount root fs
  // fprintf(stdout, "Mounting new root fs\n");
  // int mount_rc;

  // if (mount_rc = chroot("rootfs")) {
  //   fprintf(stdout, "CHROOT FAILED\n");
	// 	return mount_rc;
	// }

  // if (mount_rc = chdir("/")) {
  //   fprintf(stdout, "CHDIR FAILED\n");
	// 	return mount_rc;
	// }

  // if (mount_rc = mount("proc", "proc", "proc", 0, "")){
  //   fprintf(stdout, "MOUNT FAILED\n");
  //   return mount_rc;
  // }

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

    fprintf(stderr, "start mouting");
	if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL)) {
		fprintf(stderr, "remount failed %m\n");
		return -1;
	}
	
	char mount_dir[] = "/tmp/container.XXXXXX";
	if (!mkdtemp(mount_dir)) {
		fprintf(stderr, "make a directory failed\n");
		return -1;
	}

	char proc_path[PATH_MAX] = {0};
	snprintf(proc_path, sizeof(proc_path), "%s/proc",
                mount_dir);

	// fprintf(stderr, "Attempting to mount proc at %s\n", proc_path);
	// if (mount("", proc_path, "proc", MS_BIND | MS_PRIVATE, NULL)){
	// 	fprintf(stderr, "mount proc failed!\n");
	// 	return -1;
	// }

	if (mount("/home/yl657/Simple-Container/rootfs", mount_dir, NULL, MS_BIND | MS_PRIVATE, NULL)) {
		fprintf(stderr, "bind mount failed\n");
		return -1;
	}

	char inner_mount_dir[] = "/tmp/container.XXXXXX/oldroot.XXXXXX";
	memcpy(inner_mount_dir, mount_dir, sizeof(mount_dir) - 1);
	if (!mkdtemp(inner_mount_dir)) {
		fprintf(stderr, "make inner directory failed\n");
		return -1;
	}

	if (pivot_root(mount_dir, inner_mount_dir)) {
		fprintf(stderr, "pivot root failed\n");
		return -1;
	}

	char *old_root_dir = basename(inner_mount_dir);
	char old_root[sizeof(inner_mount_dir) + 1] = { "/" };
	strcpy(&old_root[1], old_root_dir);

	if (chdir("/")) {
		fprintf(stderr, "chdir failed %m\n");
		return -1;
	}
	if (umount2(old_root, MNT_DETACH)) {
		fprintf(stderr, "umount failed %m\n");
		return -1;
	}
	if (rmdir(old_root)) {
		fprintf(stderr, "rmdir failed %m\n");
		return -1;
	}

	fprintf(stderr, "done\n");

  return 0;
}
