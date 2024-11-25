// user/primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int valueLeft[2])__attribute__((noreturn));

void primes(int valueLeft[2]){
    int res = 0;
    if (read(valueLeft[0], &res, 4) > 0){ // is prime
        printf("prime %d\n", res);
    } else{ // end || error
        close(valueLeft[0]); 
        exit(0);
    }

    int pp[2];
    pipe(pp);
    
    if (fork() == 0) { // child 
        close(pp[1]);
        close(valueLeft[0]);
        primes(pp);
    }
    else { // parent
        close(pp[0]);

        int valueTemp = 0;
        while (read(valueLeft[0], &valueTemp, 4) != 0) { // read all
            if (valueTemp % res != 0) { // check
                write(pp[1], &valueTemp, 4); 
            }
        }

        close(pp[1]);
        wait(0);
        exit(0);
    }
}


int
main(int argc, char* argv[])
{
    int p[2];
    pipe(p);
    
    if (fork() == 0) { //  child
        close(p[1]);
        primes(p);
    }
    else {
        close(p[0]);

        int i=2;
        while (i<=280){ // parent
            write(p[1], &i, 4);
            i++;
        }

        close(p[1]);
        wait(0);
        exit(0);
    }
}