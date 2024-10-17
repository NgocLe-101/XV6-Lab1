// user/primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int valueLeft)__attribute__((noreturn));

void primes(int valueLeft){
    int res = 0;
    if (read(valueLeft, &res, 4) != 0){
        printf("prime %d\n", res);
    } else{
        exit(0);
    }

    int pp[2];
    pipe(pp);
    int value = 0;
    while (read(valueLeft, &value, 4) != 0) {
        if (value % res != 0) {
            write(pp[1], &value, 4);
        }
    }
    close(pp[1]);
    
    if (fork() == 0)
        primes(pp[0]);
    close(pp[0]);

    wait(0);
    exit(0);
}


int
main(void)
{
    int p[2];
    pipe(p);

    int i=2;
    while (i<=40){ // From 41: full
        write(p[1], &i, 4);
        i++;
    }
    close(p[1]);
    if (fork() == 0)
        primes(p[0]);
    close(p[0]);
    
    wait(0);
    exit(0);
}
