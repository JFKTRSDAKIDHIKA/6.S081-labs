#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void recursiveFind(char *, char *);

int 
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: find <directory> <filename>\n");
    return 0;
  }

  recursiveFind(argv[1], argv[2]);
  exit(0);
}

void
recursiveFind(char *directoryPath, char *targetName)
{
  struct stat st;
  int fd;
  char buf[512], *p;
  struct dirent de;

  
  if((fd = open(directoryPath, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", directoryPath);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", directoryPath);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    close(fd);
    return;
  }

  if(strlen(directoryPath) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: directoryPath too long\n");
      return;
    }
    strcpy(buf, directoryPath);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..")) == 0)
	continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if(st.type == T_DIR)
	recursiveFind(buf, targetName);
      if(st.type == T_FILE && strcmp(targetName, de.name) == 0)
	printf("%s\n", buf);
    }

  close(fd);
}
