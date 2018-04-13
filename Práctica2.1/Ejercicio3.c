#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main() {

    time_t hora;
    std::cout <<"Hora: " << time(&hora) << std::endl;
    return 1;
}