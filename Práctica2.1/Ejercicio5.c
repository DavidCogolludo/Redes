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

    int i=0;
	struct timespec t, t2; 
	//clock_gettime(CLOCK_REALTIME, &t);
	
	if(clock_gettime(CLOCK_REALTIME, &t) == 0){   
   		for (i = 0; i < 10000; ++i){}
    	clock_gettime(CLOCK_REALTIME, &t2);
	}

	std:: cout << t2.tv_nsec-t.tv_nsec <<" nanosegundos" << std::endl;
    return 1;
    

    return 0;
}