#include "sc.hpp"

static int blacklist[] = {
	SCMP_SYS(keyctl),
	SCMP_SYS(add_key),
	SCMP_SYS(request_key),
	SCMP_SYS(ptrace),
	SCMP_SYS(mbind),
	SCMP_SYS(migrate_pages),
	SCMP_SYS(move_pages),
	SCMP_SYS(userfaultfd),
	SCMP_SYS(perf_event_open),
	SCMP_SYS(personality),
	SCMP_SYS(pivot_root),
	SCMP_SYS(uselib),
	SCMP_SYS(acct), 				// gated by CAP_SYS_PACCT
	SCMP_SYS(kcmp), 				// gated by CAP_SYS_PTRACE
	SCMP_SYS(process_vm_readv), 	// gated by CAP_SYS_PTRACE
	SCMP_SYS(process_vm_writev), 	// gated by CAP_SYS_PTRACE
	// CAP_SYS_PACCT and CAP_SYS_PTRACE are not dropped
};

static int cap_gated_list[] = {
	SCMP_SYS(bpf),
	SCMP_SYS(clock_adjtime),
	SCMP_SYS(clock_settime),
	SCMP_SYS(create_module),
	SCMP_SYS(delete_module),
	SCMP_SYS(finit_module),
	SCMP_SYS(get_mempolicy),
	SCMP_SYS(init_module),
	SCMP_SYS(ioperm),
	SCMP_SYS(iopl),
	SCMP_SYS(kexec_file_load),
	SCMP_SYS(kexec_load),
	SCMP_SYS(lookup_dcookie),
	SCMP_SYS(mount),
	SCMP_SYS(name_to_handle_at),
	SCMP_SYS(open_by_handle_at),
	SCMP_SYS(quotactl),
	SCMP_SYS(reboot),
	SCMP_SYS(set_mempolicy),
	SCMP_SYS(setns),
	SCMP_SYS(settimeofday),
	SCMP_SYS(stime),
	SCMP_SYS(swapon),
	SCMP_SYS(swapoff),
	SCMP_SYS(umount),
	SCMP_SYS(umount2),
	SCMP_SYS(vm86),
	SCMP_SYS(vm86old),
};

static int obsolete_list[] = {
	SCMP_SYS(get_kernel_syms),
	SCMP_SYS(nfsservctl),
	SCMP_SYS(query_module),
	SCMP_SYS(sysfs),
	SCMP_SYS(_sysctl),
	SCMP_SYS(ustat),
};

static int extra_list[] = {
	SCMP_SYS(chmod),
	SCMP_SYS(chmod),
	SCMP_SYS(fchmod),
	SCMP_SYS(fchmod),
	SCMP_SYS(fchmodat),
	SCMP_SYS(fchmodat),
	SCMP_SYS(unshare),
	SCMP_SYS(clone),
	SCMP_SYS(ioctl),
};

static struct scmp_arg_cmp extra_cmp_list[] = {
	SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID),
	SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID),
	SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID),
	SCMP_A1(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID),
	SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISUID, S_ISUID),
	SCMP_A2(SCMP_CMP_MASKED_EQ, S_ISGID, S_ISGID),
	SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER),
	SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER),
	SCMP_A1(SCMP_CMP_MASKED_EQ, TIOCSTI, TIOCSTI),
};

int syscalls(){
	fprintf(stdout, "blocking certain syscalls\n");
	int rc = -1;
	// taking a blacklist fasion, if the syscall does not match ruls
	// below, which means they are not in the blacklist, do nothing
	scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
	if(!ctx)
		goto out;

	// SCMP_ACT_ERRNO(EPERM) make blocked syscalls return EPERM which 
	// indicates permission denied
	assert(sizeof(extra_list)/sizeof(*extra_list)==sizeof(extra_cmp_list)/sizeof(*extra_cmp_list));
	for(int i=0; i<sizeof(extra_list) / sizeof(*extra_list); i++){
		rc = seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), extra_list[i], 1, extra_cmp_list[i]);
		if(rc){ 
			fprintf(stderr, "seccomp_rule_add extra_list[%d] failed: %s\n", i, strerror(errno));
			goto out;
		}
	}

	for(int i=0; i<sizeof(blacklist) / sizeof(*blacklist); i++){
		rc = seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), blacklist[i], 0);
		if(rc){ 
			fprintf(stderr, "seccomp_rule_add blacklist[%d] failed: %s\n", i, strerror(errno));
			goto out;
		}
	}

	for(int i=0; i<sizeof(obsolete_list) / sizeof(*obsolete_list); i++){
		rc = seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), obsolete_list[i], 0);
		if(rc){ 
			fprintf(stderr, "seccomp_rule_add obsolete_list[%d] failed: %s\n", i, strerror(errno));
			goto out;
		}
	}

	// loading the seccomp filter into the kernel will fail if CAP_SYS_ADMIN 
	// is missing and NO_NEW_PRIVS has not been externally set. which means the
	// contained process cannot run binaries to regain syscalls through scmp
	rc = seccomp_attr_set(ctx, SCMP_FLTATR_CTL_NNP, 0);
	if(rc){
		fprintf(stderr, "seccomp_attr_set failed: %s\n", strerror(errno));
		goto out;
	}

	// load the filter
	rc = seccomp_load(ctx);
	if(rc)
		goto out;

	fprintf(stdout, "syscalls blocking complete\n");

out:
	if(ctx)
		seccomp_release(ctx);
	return -rc;
}