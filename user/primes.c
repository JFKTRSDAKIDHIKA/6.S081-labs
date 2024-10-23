#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter_primes(int *);

int
main(int argc, char *argv[])
{
  int p[2]; 
  uint32 n;

  pipe(p); 
  for(n = 2; n <= 35; n++)
    write(p[1], &n, 4); 
  filter_primes(p);
  exit(0);
}

void filter_primes(int *prev_p){
  if(fork() == 0){
    uint32 n; 
    int p[2];
    uint32 prime;

    pipe(p);
    close(prev_p[1]);
    if(read(prev_p[0], &prime, 4) != 0){
      if(prime == 0){
        close(prev_p[0]);
	close(p[0]);
	close(p[1]);
        exit(0);
      }  
      printf("prime %d\n", prime);
      while(read(prev_p[0], &n, 4) != 0){
        if(n % prime != 0)
          write(p[1], &n, 4); 
      }
      close(prev_p[0]);
      filter_primes(p);
    }   
  } 

  close(prev_p[0]);
  close(prev_p[1]);
  wait(0);
  exit(0);
}
