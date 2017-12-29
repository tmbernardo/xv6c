#include "types.h"
#include "defs.h"
#include "param.h"
#include "container.h"
#include "mmu.h"
#include "fs.h"
#include "proc.h"

static struct container ctable[MAXCONT];
uint total_disk = FSSIZE * BSIZE;
uint total_mem;
uint vc_num = 0;

// checks for root
int 
rootcheck(struct container *c) {
  return (strcmp(c->name, "/") == 0);
}

// finds first free location in ctable
struct container*
findfree(void) {
  int i;
  for (i = 0; i < MAXCONT; i++) {
    if (*ctable[i].name == 0) {
      return &ctable[i];
    }
  }
  return 0;
}

// finds first free console
int
findfreecons(void) {
  int i;
  for (i = 0; i < MAXCONT; i++) {
    if (ctable[i].cons == 0) {
      return i;
    }
  }
  return -1;
}

// returns index of container
int
fetchcont (char *name) {
  int i;
  for (i = 0; i < MAXCONT; i++) {
    if (strcmp(ctable[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

// finds container given name
struct container*
findname(char *name) {
  int i;
  if ((i = fetchcont(name)) < 0){
      return 0;
  }
  return &ctable[i];
}

// sets container limits
void
setlimits(struct container* c, int p, int m, int d) {
  if (p >= 0) {
    c->max_proc = p;    
  }

  if (m >= 0) {
    c->max_mem = m;
  }

  if (d >= 0) {
    c->max_disk = d;    
  }
}

// returns a container given a process
// returns 0 if process is not in a container
// doubles as an is container function
struct container*
getcont(struct proc *proc)
{
  if (proc == 0) {
      return 0;
  }
  if (proc->container != 0){
      return proc->container;
  } else {
      return getcont(proc->parent);
  }
}

int
cstop(char *name)
{
  int cont_index;
  struct container *c;

  if ((cont_index = fetchcont(name)) < 0) {
      return -1;
  }
  c = &ctable[cont_index];

  *c->name = 0;
  *c->rootpath = 0;
  c->cons = 0;
  c->rootdir = 0;
  cprocstop(c);

  return 0;
}

int
cmake(char* name, struct inode* rnode, char* rootpath) {

  struct container* c;

  if ((c = findfree()) == 0) {
    // table is full
    return -1;
  }

  strncpy(c->name, name, strlen(name));
  c->rootdir = rnode;
  strncpy(c->rootpath, rootpath, strlen(rootpath));
  c->mem_used = 0;
  c->proc_used = 0;
  c->disk_used = 0;
  c->proc_tick = 0;
  c->ticks = 0;
  setlimits(c, CDEFMAXPROC, CDEFMAXMEM, CDEFMAXDISK);

  return 1;

}

int
cinfo(void) 
{
  if (!rootcheck(myproc()->container)) {
      cprintf("ctool: info: only runnable in root\n");
      return -1;
  }

  float cpu_per;
  struct container *c;

  for (c = ctable; c < &ctable[MAXCONT]; c++ ) {
    if (*c->name != 0) {

      acquire(&tickslock);

      cpu_per = ((float) c->ticks/ticks) * 100;

      cprintf("%s\n", c->name);
      cprintf("\troot: %s\n", c->rootpath);
      cprintf("\tlimits:\n");
      cprintf("\t\tproc:\t%d\n", c->max_proc);
      cprintf("\t\tmem:\t%d\n", c->max_mem);
      cprintf("\t\tdisk:\t%d\n", c->max_disk);
      cprintf("\tused:\n");
      cprintf("\t\tproc:\t%d\n", c->proc_used);
      cprintf("\t\tmem:\t%d\n", c->mem_used);
      cprintf("\t\tdisk:\t%d\n", c->disk_used);
      cprintf("\tps:\n");
      cprintf("\t\tcpu: %%%d ((%d / %d) * 100)\n", (int) cpu_per, c->ticks, ticks);
      infocprocdump(c);

      wakeup(&ticks);
      release(&tickslock);
    }
  }

  return 0;
}
