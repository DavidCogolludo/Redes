#include "Socket.h"
#include "Serializable.h"

#include <string.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool operator== (const Socket &s1, const Socket &s2)
{
    struct sockaddr_in * a, * b;
    a = (struct sockaddr_in *)&s1.sa;
    b = (struct sockaddr_in *)&s2.sa;
    
  return(a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_family == b->sin_family &&
    a->sin_port == b->sin_port);

}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    return os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo* res;
	int rc = getaddrinfo(address, port, &hints, &res);
	//sa = res->ai_addr;
	sa_len = res->ai_addrlen;

    sd = socket(res->ai_family, res->ai_socktype, 0);

//    listen(sd, 5);
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
	obj->to_bin();

    return sendto(sock->sd, (void*)obj->data(), obj->size(), 0, (struct sockaddr *) &sock->sa, sock->sa_len);
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];

	struct sockaddr cliente;
	socklen_t cliente_len = sizeof(struct sockaddr);

	buffer = (char*)malloc(MAX_MESSAGE_SIZE);
	if(sock != 0){
		int bytes =  recvfrom(sd, (void* )&buffer, MAX_MESSAGE_SIZE, 0, &cliente, &cliente_len);

		*sock = new Socket(&cliente,cliente_len);
	}else {
		int bytes =  recvfrom(sd, (void* )&buffer, MAX_MESSAGE_SIZE, 0, &cliente, &cliente_len);
	}
    
    return 0;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

