#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <features.h>
#include <stdio.h>


int main(int argc, char** argv){

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo* res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	char buffer[80];
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	
	size_t siz = sendto(sd, argv[3], 2, 0, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	
	int bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) res->ai_addr,
   	 &res->ai_addrlen);
	buffer[bytes]='\0'; 
    
	std::cout << buffer << "\n";

	freeaddrinfo(res);
	return 0;
}