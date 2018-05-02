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
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	
	int sd = socket(res->ai_family, res->ai_socktype, 6);
	if(sd == -1){
		std::cout << "Error: " <<  gai_strerror(sd) << std::endl;
		return -1;
	}

	int error = connect(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	if(error != 0){
		std::cout << "Error: " << gai_strerror(error) << std::endl;
		return -1;
	}

	
	while(true){
		char buffer[80];
		int i = 0;
		int c = 0;
		std::cin >> buffer;

		//std::cout << buffer;
		size_t siz = send(sd, buffer, 80, 0);

		if (buffer[0] == 'q' && buffer[1] == '\0') {
			shutdown(sd, SHUT_RDWR);
			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buffer[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 79);
			break;
		}
		else {
			memset(buffer,0, 80);

			c = 0; i = 0;
			do {
			   	c = recv(sd, &(buffer[i]), 1, 0);
			   	i++;
			} while ( c >= 0 && i < 79);
			  		
			std::cout << buffer << "\n";
			memset(buffer,0, 80);
		}
	}
	freeaddrinfo(res);
	return 0;
}