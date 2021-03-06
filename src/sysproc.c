#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ptentry.h"

int
sys_fork(void)
{
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
  return kill(pid);
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

// get statistics of memory
int
sys_getpgtable(void)
{
  // implement
  int num;
  struct pt_entry *entries;
  int wsetOnly;

  if (argint(1, &num) < 0 || argptr(0, (void *)&entries, sizeof(entries)) < 0 || argint(2, &wsetOnly) < 0) {
    // something is invalid
    return -1;
  }

  return getpgtable(entries, num, wsetOnly);
}

// allows the user to dump the raw content of one physical page where physical_addr resides
int
sys_dump_rawphymem(void) {
  // TODO: implement
  // DO NOT USE ARGPTR HERE
  int physical_addr;
  char *buffer;
  
  if (argint(0, &physical_addr) < 0 || argptr(1, &buffer, PGSIZE) < 0) {
    return -1;
  }
  return dump_rawphymem((uint)physical_addr, buffer);
}