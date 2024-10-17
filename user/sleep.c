#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks;

  if(argc != 2){
    fprintf(2, "Invalid format! Use: sleep <number of ticks>\n");
    exit(1);
  }

  ticks = atoi(argv[1]);
  if(ticks <= 0){
    fprintf(2, "sleep: invalid number of ticks\n");
    exit(1);
  }

  sleep(ticks);
  exit(0);
}