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

size_t dameFecha(char comand, char *buffer){
	time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
	if(comand == 't')
  	return strftime (buffer,80,"%r",timeinfo);
	else if(comand == 'd')
		return strftime (buffer,80,"%F",timeinfo);
	else if(comand == 'q')
		return 0;
	else{
		std::cout << "Comando no soportado: " << comand << "\n";
		return 81;
	}  //puts (comand);
}

class ServerThread{
public:
	ServerThread(int s): sd(s){}
	virtual ~ServerThread(){close(sd);}

	void do_message(){

		char buffer[80];
		struct sockaddr cliente;
		
		socklen_t cliente_len = sizeof(struct sockaddr);
		
		char host[NI_MAXHOST];
		char serv [NI_MAXSERV];

		while (1) {
	 		int bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) &cliente,
	    	 &cliente_len);
	  		buffer[bytes]='\0'; 
	    
	  		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
	    	  serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

	  		std::cout << "Thread: " << pthread_self() << "\n";
	  		std::cout << "Conexión desde Host: "<< host << " Puerto: "<< serv<<"\n";
	  		std::cout<< "\tMensaje(" << bytes <<"): " << buffer << "\n";

	  		if(bytes > 2) std::cout << "Comando no soportado: " << buffer << "\n";
	  		else {
		  		size_t siz = dameFecha(buffer[0], buffer);
		  		if(siz != 81)
		  			sendto(sd, buffer, siz, 0, (struct sockaddr *) &cliente, cliente_len);

	  		}
	  		sleep(3);
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
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo* res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}
	int sd = socket(res->ai_family, res->ai_socktype, 0);

	bind(sd,res->ai_addr, res->ai_addrlen);

	pthread_t tid[NUM_THREADS];	
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_attr_t attr;

		ServerThread* st = new ServerThread(sd);
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid[i], &attr, start_routine, static_cast<void*>(st));
	}

	char c;
	do std::cin >> c;
	while (c != 'q');

	freeaddrinfo(res);
	exit(0);
	return 0;
}