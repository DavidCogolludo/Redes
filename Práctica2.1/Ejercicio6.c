#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#include <time.h>
#include <features.h>

//Compilar con -lrt al final
int main() {

	time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"Estamos en el año %G.\nHoy es %A, %R.",timeinfo);
  puts (buffer);

    return 0;
}