// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "param.h"

char *cont_create_exec[] = {"ctool", "create", "/", 0};
char *cont_start_exec[] = {"sh", 0};

int
main(void)
{
  int pid, wpid, fd;

  pid = fork();

  // create root container
  if (pid == 0) {
      exec("ctool", cont_create_exec);
      printf(1, "init: container creation in root failed\n");
      exit();
  }
  wait();

  // make root container's vc (special case)
  if((fd = open("vc0", O_RDWR)) < 0){
    mknod("vc0", 1, 1);
    fd = open("vc0", O_RDWR);
  }
  vcmake();

  climits("/", NPROC, tmem(), tdisk());
  ccons("/", "vc0");   
  
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
      printf(1, "init: starting sh in root container\n");
      pid = cfork("/");
      if(pid < 0){
           printf(1, "init: fork failed\n");
           exit();
      }
      
      if(pid == 0){
          exec("sh", cont_start_exec);
          printf(1, "init: starting sh in root container failed\n");
          exit();
      }
      while((wpid=wait()) >= 0 && wpid != pid) {
          printf(1, "zombie!\n");
      }
  }
}
