#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf(2, "Usage: Invalid arguments\n");
        exit();
    }

    int pid = atoi(argv[1]);
    int position = fifo_position(pid);

    if (position < 0)
    {
        printf(1, "-1\n");
    }
    else
    {
        printf(1, "fifo Position: %d: \n", position);
    }

    exit();
}

