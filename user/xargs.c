#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char *exec_argv[MAXARG];  

  for(int i = 0; i < MAXARG; i++) {
    exec_argv[i] = malloc(100 * sizeof(char));
    if(exec_argv[i] == 0) {
      printf("Memory allocation failed\n");
      exit(1);
    }
  }

  char buf[100];
  char *buffer_pos;
  
  for(int i = 1; i < argc; i++)
    strcpy(exec_argv[i - 1], argv[i]);
  
  buffer_pos = buf;
  while(read(0, buffer_pos, sizeof(char)) == sizeof(char)){
    if(*buffer_pos == '\n'){
      *buffer_pos = 0;
      strcpy(exec_argv[argc - 1], buf);
      exec_argv[argc] = 0;
      buffer_pos = buf;
      if(fork() == 0){
        exec(exec_argv[0], exec_argv);
        fprintf(2, "exec %s failed\n", exec_argv[0]);
        exit(1);  
      }
      wait(0);
      continue;
    }
    buffer_pos++;
  }
  
  exit(0);
}
