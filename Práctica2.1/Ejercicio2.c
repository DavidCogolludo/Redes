#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>

int main() {

    if(setuid(0) == -1){
    	
    	std::cout << "Error: " << errno << " "<< strerror(errno) << std::endl;
    }
    return 1;
}