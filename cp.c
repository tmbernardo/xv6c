#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"

#define USAGE "cp <src> <dest>"

int
main(int argc, char *argv[])
{
  char buf[512];
  int sfd, dfd, r, w;
  char *src;
  char *dest;

  if(argc != 3){
    printf(1, "%s\n", USAGE);
    exit();
  }

  src = argv[1];
  dest = argv[2];

  if((sfd = open(src, O_RDONLY)) < 0){
    printf(1, "cp: cannot open src: %s\n", src);
    exit();
  }

  if((dfd = open(dest, O_WRONLY|O_CREATE)) < 0){
    printf(1, "cp: cannot open dest: %s\n", dest);
    exit();
  }

  while ((r = read(sfd, buf, sizeof(buf))) > 0) {
    w = write(dfd, buf, r);
    if (w != r || w < 0) 
      break;
  }

  if (r < 0 || w < 0) {
    printf(1, "cp: error copying %s to %s\n", src, dest);
  }

  close(sfd);
  close(dfd);

  exit();
}
