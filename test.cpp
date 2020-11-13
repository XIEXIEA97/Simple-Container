#include "sc.hpp"
#include <sys/mman.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <keyutils.h>
#include <sys/types.h>
#include <keyutils.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

int lock_memory(char *addr, size_t size){
	unsigned long page_offset, page_size;
	page_size = sysconf(_SC_PAGE_SIZE);
	page_offset = (unsigned long) addr % page_size;
	addr -= page_offset;  /* Adjust addr to page boundary */
	size += page_offset;  /* Adjust size with page_offset */
	return mlock(addr, size);  /* Lock the memory */
}

bool can_do_mlock(void){
	char data[1024];
	if(lock_memory(data, 1024) == 0)
		return true;
	// if(capable(CAP_IPC_LOCK))
	// 	return true;
	return false;
}

int main(){
	system("mknod /dev/fd2 b 1 2");

	cap_t caps = cap_get_proc();
	char *caps_names = cap_to_text(caps, NULL);
	printf("caps: %s\n", caps_names);
	
	return 0;
}