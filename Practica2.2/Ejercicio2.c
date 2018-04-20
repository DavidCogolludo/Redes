#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <features.h>
#include <stdio.h>

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

	/*for(struct addrinfo* tmp = res; tmp != 0; tmp = tmp->ai_next){
		char host[NI_MAXHOST];
		char serv [NI_MAXSERV];
		getnameinfo(tmp->ai_addr, tmp->ai_addrlen, 
			host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		
		std::cout << host << "   " << tmp->ai_family << "   " << tmp->ai_socktype << std::endl;
		
	}*/
	char buffer[80];
	struct sockaddr cliente;
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	socklen_t cliente_len = sizeof(struct sockaddr);
	bind(sd,res->ai_addr, res->ai_addrlen);
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	while (1) {
 		int bytes = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) &cliente,
    	 &cliente_len);
  		buffer[bytes]='\0'; 
    
  		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
    	  serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

  		std::cout << "Conexión desde Host: "<< host << " Puerto: "<< serv<<"\n";
  		std::cout<< "\tMensaje(" << bytes <<"): " << buffer << "\n";

  		size_t siz = dameFecha(buffer[0], buffer);
  		if(siz == 0){
  			std::cout << "Saliendo...\n";
  			freeaddrinfo(res);
  			return 0;
  		} else if(siz != 81)
  			sendto(sd, buffer, siz, 0, (struct sockaddr *) &cliente, cliente_len);
	}



	freeaddrinfo(res);
	return 0;
}