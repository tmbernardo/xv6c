#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "container.h"

int
sys_fork(void)
{
  struct container *cont = getcont(myproc());

  if (cont != 0) {
    return cfork(cont);
  }
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;

  struct container *cont = getcont(myproc());

  if (cont != 0 && !rootcheck(cont)) {
    return ckill(pid);
  } else {
    return kill(pid);
  }
}

int
sys_getticks(void)
{
    return myproc()->ticks;
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// get used memory
int
sys_umem(void)
{
  return umem(myproc()); 
}

// get total memory
int
sys_tmem(void)
{
  return tmem(myproc());
}

// get used disk
int
sys_udisk(void)
{
  return udisk(myproc()); 
}

// get total disk
int
sys_tdisk(void)
{
  return tdisk(myproc());
}

// print processes
void
sys_cprocdump(void)
{
  cprocdump(myproc());
}

