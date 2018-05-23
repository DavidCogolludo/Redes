#include <string>
#include <unistd.h>
#include <string.h>
#include "Serializable.h"
#include "Socket.h"
#include "UDPServer.h"

class ChatMessage: public Serializable
{
public:
    ChatMessage(){};

    ChatMessage(const char * n, const std::string m)
    {
    	strncpy(nick, n, 8);

    	m.copy(message,80);
    };

    void to_bin()
    {
    	 int32_t total = 80 * sizeof(char) + 2 * sizeof(int16_t);
    	 alloc_data(total);

    	 char * tmp = _data + sizeof(int32_t);
    	 memcpy(tmp, (void*)nick, 8);
    	 tmp+= 8;
    	 memcpy(tmp, (void*)message, 80);
    	 tmp += 80;
    }

    virtual int from_bin(char * bobj)
    {
    	char *tmp = bobj + sizeof(int32_t);
    	memcpy(nick, tmp, 8);
    	tmp+= 8;
    	memcpy(message, tmp, 80);
    	tmp += 80;

    	return 0;
    }

    char message[80];

    char nick[8];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ChatServer : public UDPServer
{
public:
    ChatServer(const char * s, const char * p): UDPServer(s, p){};

    void do_message(char * buffer)
    {
    	ChatMessage m;
    	m.from_bin(buffer);

    	for(int i = 0; i < connections.size(); i++){
    		socket.send(&m, connections[i]);
    	}

    }
};

class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p)
        {
        		strncpy(nick, n, 8);
        };

    void input_thread()
    {
    	char c[80];
    	ChatMessage* m;
    	while(1){

    	    std::cin >> c;
    	    m = new ChatMessage(nick, c);
    	    m->to_bin();
    	    socket.send(m, &socket);
    	}
    }

    void net_thread()
    {
    	char buf[80];
    	ChatMessage m;

    	while(1){

    	    socket.recv(buf);
    	    m.from_bin(buf);

    	    if(m.nick != nick){
    			std::cout << m.nick <<  ": "<< m.message;
    	    }
    	}
    }

private:
    char nick[8];

    Socket socket;
};

