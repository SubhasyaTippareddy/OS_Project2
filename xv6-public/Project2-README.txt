Project 2 README

Group Members:
Subhasya Tippareddy
Mounika Boggavarapu
Sri Harsha Mulluri

OS:Ubuntu 22.04.3 LTS
Processor: intel i7
RAM:16 GB

ticks_running():
To implement ticks_running, we define stime for proc structure in proc.h header file and is initialized to 0 in allocproc() when a process is created. When the system call ticks_running() is called with pid, it searches for the given pid in the process table and return the number of ticks a process is scheduled in the cpu. 

If the process with the given pid is not present in the process table, the system call returns -1. If a process exists and is not yet scheduled then it returns 0. 

Simple Scheduler:

We choose to implement FIFO scheduler which schedules the processes in the order of their arrival times. 
The relevant code to implement FIFO is added in scheduler() function of proc.c file. We defined arrival_time in the proc structure to keep track of arrival times of the processes. 
The FIFO scheduler goes through the process table to find process with least arrival time. We check if the process is in RUNNABLE state and if yes, allocates cpu to it and repeats this for the remaining processes. 

Additionally, we implemented a system call fifo_position. We define a fifoposition in the process structure in proc.h header file. Everytime a new process is entering, it is assigned a position which is assigned to fifoposition variable. The system call fifo_position() takes pid of a process as input, and scans through the process table for the pid and returns the fifoposition. 

Advanced Scheduler:

Among the given options, we choose to implement priority scheduler. Here, we define a priority variable for a proc in proc.h header file
The scheduler schedules the process based on the priorities assigned. We have 3 levels (Low - 3, Mid - 2 (default), High -1). Higher the value, lower the priority.

We also implement two system calls as instructed - 'set_sched_priority' which is called by a process with a priority as input. The system call looks through the table and finds the process with pid and assigns the priority to the process. The other system call, 'get_sched_priority' takes pid of a process as input and puts a lock on the process table to scan it and finds the process with the pid and returns it's priority. If the process is not found, we return -1. 



We additionally use commmands - foo and listruns for only testing purposes. 'foo' will create a child process using fork and 'listruns' will return the processes in the table - except which is in UNUSED state. 


NOTE:
In part 4 of the project, we tried to implement stressfs command but this almost everytime resulted in the following error - 'lapicid 0: panic: balloc: out of blocks'. This panic message apparently indicates that the OS is running out of disk blocks for some reason. We investigated our best but were not able to resolve this issue and hence opted to implemented a different user call. 





