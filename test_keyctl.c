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

int can_keyctl(){
	key_serial_t key_to_instantiate = atoi("mykey");
	if(keyctl(KEYCTL_ASSUME_AUTHORITY, key_to_instantiate) == -1){
		printf("KEYCTL_ASSUME_AUTHORITY failed: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}

int main(){
	printf("try to call keyctl(): ... %s\n", (can_keyctl() ? "failed" : "success"));
	return 0;
}
