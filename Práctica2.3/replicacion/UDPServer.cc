#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);
};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{
	pthread_t tid[THREAD_POOL_SIZE];	
	for(int i = 0; i < THREAD_POOL_SIZE; i++){
		pthread_attr_t attr;

		UDPServer* st = new UDPServer(socket);
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid[i], &attr, _server_thread, static_cast<void*>(st));
	}
}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{ 	while(true){

		char buffer [256];
		Socket * cliente;

		socket.recv(buffer, &cliente); 

		connections.push_back(cliente);
	    
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
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

