#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n;

  if(argc <= 1){
    fprintf(2, "sleep: missing argument\n");
    exit(1);
  }
  
  
  if(argc >= 3){
    fprintf(2, "sleep: too many arguments\n");
    exit(1);
  }
  
  n = atoi(argv[1]);
  if(sleep(n) < 0){
    fprintf(2, "sleep: system call failed\n");
    exit(1);
  }
  
  exit(0);
}
