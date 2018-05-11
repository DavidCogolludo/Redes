#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#define NUM_THREADS  5

class ServerThread{
public:
	ServerThread(int s): sd(s){}
	virtual ~ServerThread(){}

	void do_message(){

		char buffer[80];

		while (true){
	  		int c = 0; int i = 0;
	  		do {
	        	c = recv(sd, &(buffer[i]), 1, 0);
	  		} while ( c >= 0 && i < 79 && buffer[i++] != '\n');
	  		
	  		if(c == 0)
	  		{
	  			close(sd);
	  			std:: cout << "Conexión terminada\n";
	  			pthread_cancel(pthread_self());
	  			break;
	  		} 
	  		send(sd, buffer, i, 0);
  		}
  		
	}
private:
	int sd;
};

    extern "C"
    {
        void* start_routine(void* _st){
			ServerThread * st = static_cast<ServerThread*>(_st);
			st->do_message();
			delete st;
			return 0;
		}
        /* add other C function prototypes here if needed */
    }


int main(int argc, char** argv){

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

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
	
	freeaddrinfo(res);

	while(true){

		
  		cliente_sd = accept(sd, (struct sockaddr *) &cliente, &cliente_len);
  		/*if (cliente_sd == -1){
			printf("Error %s\n", );

  		}*/
  		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, 
  			NI_NUMERICHOST|NI_NUMERICSERV);
  
  		printf("Conexión desde Host:%s Puerto:%s\n",host, serv);
  		
  		pthread_t tid;
  		pthread_attr_t attr;
		ServerThread* st = new ServerThread(cliente_sd);
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));

  		}
  		exit(0);
  		return 0;
	}
	