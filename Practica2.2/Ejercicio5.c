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
	
	int error = 0;
	error = connect(sd,res->ai_addr, res->ai_addrlen);
	std::cin >> buffer;
	int i = 0;
	do{
		size_t siz = send(sd, buffer, i, 0);
		i++;
	} while(i < 80);
	
	int c = 0; i = 0;
	do {
	   	c = recv(sd, &(buffer[i]), 1, 0);
	} while ( c >= 0 && i < 79 && buffer[i++] != '\n');
	  		
    
	std::cout << buffer << "\n";

	freeaddrinfo(res);
	return 0;
}