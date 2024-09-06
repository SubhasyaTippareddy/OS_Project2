#include "types.h"
#include "user.h"
// read the arguments of the command
int main(int argc, char *argv[]) {
  int time;
  // if argument count is less than 2 then print error
  if(argc < 2 || argc >2) {
    printf(2, "Error: too many or too less arguments\n");
    exit();
  }
  // read the ticks if the time is negative ask to give positive time
  time = atoi(argv[1]);
  if(time <= 0) {
    printf(2, "Error: Please give the positive sleep time\n");
    exit();
  }
  // or else call the sleep command using sleep function.
  sleep(time);
  exit();
}
