#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
// This is just to create a dummy process
int main(int argc, char *argv[])
{
    int k, n, id;
    double x=0, z;

    if(argc < 2)
        n = 1;  
    else
        n = atoi(argv[1]); 
    if(n<0 || n>100)
        n = 2;

    x = 0;
    id = 0;
    for(k=0; k<n; k++)
    {
        id = fork();
        if(id < 0)
            printf(1, "%d failed in fork!\n", getpid());
        else if(id > 0)
        {  
            printf(1, "Parent %d creating child %d\n", getpid(), id);
            wait();
        }
        else
        {   // Child
            printf(1, "Child %d created\n", getpid());
            for(z=0;z<8000000.0;z+=0.001)
                x = x + 3.14*69.69; 
            break;
            exit();
        }
    }
    exit();
}