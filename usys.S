#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALL(cmake)
SYSCALL(climits)
SYSCALL(ccons)
SYSCALL(cfork)
SYSCALL(cprocdump)
SYSCALL(vcmake)
SYSCALL(umem)
SYSCALL(tmem)
SYSCALL(getticks)
SYSCALL(cinfo)
SYSCALL(udisk)
SYSCALL(tdisk)
SYSCALL(cpause)
SYSCALL(cresume)
SYSCALL(cstop)
