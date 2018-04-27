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
	hints.ai_socktype = SOCK_STREAM;;

	struct addrinfo* res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd,res->ai_addr, res->ai_addrlen);

	listen(sd, 5);
	
	int cliente_sd;
	struct sockaddr cliente;
	socklen_t cliente_len = sizeof(struct sockaddr);
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	char buffer[80];


	while(true){

		
  		cliente_sd = accept(sd, (struct sockaddr *) &cliente, &cliente_len);
  		/*if (cliente_sd == -1){
			printf("Error %s\n", );

  		}*/
  		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, 
  			NI_NUMERICHOST|NI_NUMERICSERV);
  
  		printf("Conexión desde Host:%s Puerto:%s\n",host, serv);
  		while (true){
	  		int c = 0; int i = 0;
	  		do {
	        	c = recv(cliente_sd, &(buffer[i]), 1, 0);
	  		} while ( c >= 0 && i < 79 && buffer[i++] != '\n');
	  		
	  		if(c == 0)
	  		{
	  			close(cliente_sd);
	  			break;
	  		} 
	  		send(cliente_sd, buffer, i, 0);
  		}
  		std:: cout << "Conexión terminada\n";
	}
	
	freeaddrinfo(res);
	return 0;
}