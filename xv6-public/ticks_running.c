#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf(2, "Usage: ticks_running pid\n");
    exit();
  }
  int pid = atoi(argv[1]);
  int scheduled_time = ticks_running(pid);
  if(scheduled_time < 0){
    printf(2, "process does not exist with pid %d: %d\n", pid, scheduled_time);
    exit();
  }
  if(scheduled_time == 0){
    printf(2, "unscheduled process with pid %d: %d\n", pid, scheduled_time);
    exit();
  }
  
  printf(1, "Scheduled time for process %d: %d\n", pid, scheduled_time);
  exit();
}
