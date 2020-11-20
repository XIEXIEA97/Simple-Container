TARGETS = hostname.sh mountkernfs.sh plymouth-log udev urandom hwclock.sh mountdevsubfs.sh checkroot.sh mountnfs-bootclean.sh mountnfs.sh bootmisc.sh mountall-bootclean.sh mountall.sh checkfs.sh checkroot-bootclean.sh procps
INTERACTIVE = udev checkroot.sh checkfs.sh
udev: mountkernfs.sh
urandom: hwclock.sh
hwclock.sh: mountdevsubfs.sh
mountdevsubfs.sh: mountkernfs.sh udev
checkroot.sh: hwclock.sh mountdevsubfs.sh hostname.sh
mountnfs-bootclean.sh: mountnfs.sh
bootmisc.sh: mountnfs-bootclean.sh udev mountall-bootclean.sh checkroot-bootclean.sh
mountall-bootclean.sh: mountall.sh
mountall.sh: checkfs.sh checkroot-bootclean.sh
checkfs.sh: checkroot.sh
checkroot-bootclean.sh: checkroot.sh
procps: mountkernfs.sh udev
