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
}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(address, port, &hints, &sa);
    sd = socket(sa->ai_family, sa->ai_socktype, 0);

    listen(sd, 5);
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
    return send(sock, obj->data(), obj->size(), 0);
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
    int c = 0; int i = 0;
    
        c = recvfrom(sock, &(buffer[i]), 1, 0, addr, addrlen);
    
    *sock = new Socket(addr, addrlen)           
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

