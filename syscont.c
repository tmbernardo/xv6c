#include "types.h"
#include "defs.h"
#include "param.h"
#include "container.h"
#include "mmu.h"
#include "proc.h"

// makes container (name & rootdir)
int 
sys_cmake(void) {
  char *name, *rootdir;
  struct inode *rnode;

  if(argstr(0, &name) < 0 || argstr(1, &rootdir) < 0)
    return -1;

  if (name == 0 || (rnode = namei(rootdir)) == 0) {
    return -1;
  }

  return cmake(name, rnode, rootdir);
}

// sets container limits
int 
sys_climits(void) {
  char *name;
  int p = -1, m = -1, d = -1;
  struct container *c;

  if(argstr(0, &name) < 0 || argint(1, &p) < 0 || argint(2, &m) < 0 || argint(3, &d) < 0)
    return -1;

  if (name == 0) {
    // bad params
    return -1;
  }

  if ((c = findname(name)) == 0) {
    return -1;
  }

  setlimits(c, p, m, d);
  return 1;
}

// sets container console
int 
sys_ccons(void) {
  char *name, *con;
  struct inode *cnode;
  struct container *c;

  if(argstr(0, &name) < 0 || argstr(1, &con) < 0)
    return -1;

  if (name == 0 || (cnode = namei(con)) == 0) {
    // bad params
    return -1;
  }

  if ((c = findname(name)) == 0) {
    return -1;
  }

  strncpy(c->consname, con, strlen(con));

  c->cons = cnode;
  return 1;
}

// checks disk usage and starts/forks process
int
sys_cfork(void) {
  char *name;
  struct container *c;

  if(argstr(0, &name) < 0)
    return -1;

  if ((c = findname(name)) == 0) {
    return -1;
  }

  if (c != 0) {
    c->disk_used = dirsize(c->rootdir);

    if (c->disk_used > c->max_disk) {
      cprintf("container out of disk space!\n");
      return -1;
    }
  }

  return cfork(c);
}

// pauses a container and its processes
int
sys_cpause(void) {
  struct container *c;
  char *name;
  
  if(argstr(0, &name) < 0)
    return -1;
  if((c = findname(name)) == 0) {
    return -1;
  }
  
  return cpause(c);
}

// resumes a container and its processes
int
sys_cresume(void) {
  struct container *c;
  char *name;
  
  if(argstr(0, &name) < 0)
    return -1;
  if((c = findname(name)) == 0) {
    return -1;
  }
  return cresume(c);
}

// stops a container and its processes
int
sys_cstop(void) {
  char *name;

  if(argstr(0, &name) < 0)
    return -1;

  return cstop(name);
}

// find free console and increment number of vcs
int
sys_vcmake(void)
{
  int vc;
  vc = findfreecons();
  if (vc + 1 > vc_num) {
    vc_num = vc + 1;
  }
  return vc;
}

// print out container info
int
sys_cinfo(void)
{
  return cinfo();
}
