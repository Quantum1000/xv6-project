#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

int pid[3];
int ticks[3];
int dt[3];

void
calc(struct pstat *ps)
{
  int i;
  for(i = 0; i < NPROC; i++)
  {
    int j;
    for(j = 0; j < 3; j++)
    {
      if(pid[j] == ps->pid[i])
      {
        dt[j] = ps->ticks[i] - ticks[j];
        ticks[j] = ps->ticks[i];
	break;
      }
    }
  }
}

int
main(int argc, char *argv[])
{
  struct pstat ps;
  int i;
  printf(1, "graph starting\n");
  for(i = 0; i < 3; i++)
    ticks[i] = 0;
  settickets(10);
  if((pid[0] = fork()) == 0)
  {
    for(;;)
      i = i;
  }
  settickets(30);
  if((pid[1] = fork()) == 0)
  {
    for(;;)
      i = i;
  }
  settickets(20);
  if((pid[2] = fork()) == 0)
  {
    for(;;)
      i = i;
  }
  printf(1, "graph processes initialized, 1: %d, 2: %d, 3: %d\n", pid[0], pid[1], pid[2]);
  settickets(10000);
  if(getpinfo(&ps) < 0)
    printf(1, "graph: unknown error\n");
  calc(&ps);
  if(argc > 1)
    printf(1, "graph: does not take arguments\n");
  char out[47];
  out[46] = 0;
  for(i = 0; i < 46; i++)
  {
    if(i%5 == 0)
    {
      out[i] = i/5+'0';
    }
    else
    { 
      out[i] = 196;
    }
  }
  printf(1, out);
  printf(1, "\n");
  out[30] = 0;
  for(i = 0; i < 10; i++)
  {
    sleep(60);
    getpinfo(&ps);
    calc(&ps);
    int j;
    for(j = 0; j < 46; j++)
    {
      int k;
      out[j] = ' ';
      if(j%5 == 0)
        out[j] = '|';
      for(k = 0; k < 3; k++)
      {
        if(dt[k]/2 == j)
	  out[j] = k + '1';
      }
    }
    printf(1, out);
    printf(1, " 1: %d, 2: %d, 3: %d", dt[0], dt[1], dt[2]);
    printf(1, "\n");
  }
  enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  for(i = 0; i < NPROC; i++)
  {
    if(ps.inuse[i])
    {
      printf(1, "pid: %d, state: %s, ticks: %d, tickets: %d\n", ps.pid[i], states[ps.inuse[i]],  ps.ticks[i], ps.tickets[i]);
    }
  }
  kill(pid[0]);
  kill(pid[1]);
  kill(pid[2]);
  exit();
}
