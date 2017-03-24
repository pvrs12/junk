#include <stdio.h>


#include <unistd.h>

#define __USE_GNU
#include <dlfcn.h>
ssize_t write(int fd, const void* buf, size_t count){
	static ssize_t (*write_real)(int, const void*, size_t) = NULL;

	if(!write_real){
		write_real = dlsym(RTLD_NEXT, "write");
	}
	printf("Haxx: '%.*s'\n", count, buf);

	write_real(fd, buf, count);
}
