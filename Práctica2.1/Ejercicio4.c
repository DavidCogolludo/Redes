#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main() {

    time_t hora;
    time (&hora);
    char bf[90]; //El espacio se reserva en el main
    //y por eso el programa funciona con threads.
    ctime_r(&hora, bf);
    std::cout <<"Hora: " << bf << std::endl;

    //Si en lugar de ctime_r() se usara ctime(), no se podrían utilizar threads.
    //Ya que devuelve un puntero que se ha creado en la pila.
    return 1;
}