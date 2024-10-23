#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void panic(char *);

int 
main(int argc, char* argv[])
{
  int p[2];
  char buf[1];

  if(argc > 1)
    panic("pingpong: too many arguments");

  if(pipe(p) < 0)
    panic("pipe");
  if(fork() == 0){
    close(0);
    dup(p[0]);
    close(p[0]);
    read(0, buf, 1);
    printf("%d: received ping\n", getpid());
    close(1);
    dup(p[1]);
    close(p[1]);  
    write(1, buf, 1);
    close(0);
    close(1);
    exit(0); 
  }
  write(p[1], buf, 1);
  close(p[1]);
  close(0);
  dup(p[0]);
  close(p[0]);
  read(0, buf, 1);
  close(0);
  printf("%d: received pong\n", getpid());
  exit(0);
}

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}
