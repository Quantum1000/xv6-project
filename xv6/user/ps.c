#include "types.h"
#include "stat.h"
#include "user.h"


int
main(int argc, char *argv[])
{
  enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  struct pstat ps;
  
  if(getpinfo(&ps) < 0)
    printf(1,  "ps: retrieval error\n");
   
  int i = 0;
  for(i = 0; i < NPROC; i++)
  {
    if(ps.inuse[i])
    {
      printf(1, "pid: %d, state: %s, ticks: %d, tickets: %d\n", ps.pid[i], states[ps.inuse[i]], ps.ticks[i], ps.tickets[i]);
    }
  }
  exit();
}
