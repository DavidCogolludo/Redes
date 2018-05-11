#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        char * dest;
        memcpy((void*)dest, (void*) name, sizeof(char)*80);

        memset()

    }

    int from_bin(char * data)
    {
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{

    char name[80];
    int16_t x;
    int16_t y;

    std::cin >> name;
    std::cin >> x;
    std::cin >> y;
    Jugador one(name,x,y);

    int f;
    creat("Jugador",S_IRWXU | S_IRWXO);
    one.to_bin();
    write(f, one.data(), one.size());
    


}