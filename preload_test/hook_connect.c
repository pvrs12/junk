#include <stdio.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

#include <errno.h>
#ifndef EACCESS
#define EACCESS 13
#endif

#define __USE_GNU
#include <dlfcn.h>

int connect(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen) {
	static int (*connect_real)(int, const struct sockaddr*, socklen_t) = NULL;
	const unsigned char* c;
	int port, ok=1;

	if (!connect_real){
		connect_real = dlsym(RTLD_NEXT,"connect");
	}

	if (serv_addr->sa_family == AF_INET6){
		return EACCESS;
	}

	if (serv_addr->sa_family == AF_INET){
		c = serv_addr->sa_data;
		port=256*c[0]+c[1];
		c+=2;
		ok=0;

		//if to 127.0.0.1
		if((*c == 127) && 
			(*(c + 1) == 0) && 
			(*(c + 2) == 0) && 
			(*(c + 3) == 1)){
			ok = 1;
		}
		//or any 8080
		if (port == 8080){
			ok = 1;
		}
	}
	if(ok){
		//perform the actual connection
		printf("You're good to go\n");
		return connect_real(sockfd, serv_addr, addrlen);
	} else {
		printf("Fuck off doode\n");
	}

	if(getenv("WRAP_TCP_DEBUG")){
		fprintf(stderr, "connect() denied to address %d.%d.%d.%d:%d\n", (int)(*c),(int)(*(c+1)),(int)(*(c+2)),(int)(*(c+3)), port);
	}
	return EACCES;
}
