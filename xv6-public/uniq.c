#include "types.h"
#include "stat.h"
#include "user.h"
// create a buffer with max length
char buffer[512];
char previousbuffer[512];
char currentbuffer[512];                                       

// string comparision for n characters for -w flag
int strncmp(const char *string1, const char *string2, int n) {
    while (n && *string1 && (*string1 == *string2)) {                         
        n--;
        string1++, string2++;
    }
    if (n == 0) return 0;
    return (uchar)*string1 - (uchar)*string2;
}

// The uniq function
void uniq(int fd, int flag, int w) {
  int n, m = 0, loop = 0;
  int count, linecount = 1;                                       
  char *p, *x;

  // initialize the prevbuf
  memset(previousbuffer, 0, sizeof(previousbuffer));
  // initialize a variable to track the first line
  int fl = 1;

  // Read each line of a file
  while((n = read(fd, buffer, sizeof(buffer))) > 0) {
    x = buffer;
    while(strlen(buffer) != m){
      count = 0;
      for(p = x; *p != '\n'; p++) count++;
      p++;
      memmove(currentbuffer, x, count);
      currentbuffer[count] = 0;
      x = x + count + 1;
      m = m + count + 1;

      // implementing -u flag
      if (flag == 2) {  
        if(strcmp(previousbuffer, currentbuffer) == 0) {
          linecount++;
        } else {
          // finding the lines only occurs once
          if(linecount == 1 && !fl) {
            write(1, previousbuffer, strlen(previousbuffer));
            printf(1, "\n");
          }
          strcpy(previousbuffer, currentbuffer);
          linecount = 1;
        }
        fl = 0;
      } 
      // implementing -c flag
      else if (flag == 1) { 
        if(strcmp(previousbuffer, currentbuffer) == 0) {
          // counting the occurence of the line
          linecount++;
        } else {
          if(loop > 0) {
            printf(1, "%d %s\n", linecount, previousbuffer);
          }
          strcpy(previousbuffer, currentbuffer);
          linecount = 1;
        }
        // implementing the -w flag
      } else if (flag == 3) {
        // string comparision with n characters
        if (strncmp(previousbuffer, currentbuffer, w) == 0) {
          linecount++;
        } else {
          if(loop > 0) {
            printf(1, "%s\n", previousbuffer);
          }
          strcpy(previousbuffer, currentbuffer);
          linecount = 1;
        }
      } 
      // general uniq or cat uniq command implementation
      else {
        if(strcmp(previousbuffer, currentbuffer) == 0) {
          linecount++;
        } else {
          if(loop > 0) {
            printf(1, "%s\n", previousbuffer);
          }
          strcpy(previousbuffer, currentbuffer);
          linecount = 1;
        }
      }
      loop++;
    }

    // Handle the last line for -c, -u and -w
    if ((flag == 1 || flag == 2 || flag == 3 || flag==0) && linecount >= 1) {
      if (flag == 1) {
        printf(1, "%d %s\n", linecount, previousbuffer);
      } else {
        write(1, previousbuffer, strlen(previousbuffer));
        printf(1, "\n");
      }
    }
  }

  if(n < 0){
    printf(1, "uniq: read error\n");
    exit();
  }
}

// Main function
int main(int argc, char *argv[]) {
  int fd;
  char *option;
  //initialize w for strong number in -w flag
  int w = 0;

  if(argc <= 1){
    // Read from standard input
    uniq(0, 0, 0);  
    exit();
  }

  // -w flag in format [N]
  if (argc == 4 && strcmp(argv[1], "-w") == 0 && argv[2][0] =='[') {
    char *arg = argv[2];
    int len = strlen(arg);

    // check the argv[2] is in format [N]
    if (arg[0] == '[' && arg[len - 1] == ']') {
        arg[len - 1] = '\0'; 

        // Convert to integer from index 1
        w = atoi(arg + 1);
    }
    // if file was unable to open give error
    if((fd = open(argv[3], 0)) < 0){
      printf(1, "unable to open the file %s\n", argv[3]);
      exit();
    }
    // call uniq function with -w flag
    uniq(fd, 3, w);
    close(fd);
    exit();
  }
  // for -w flag argv[2] with format N
  else if(argc == 4 && strcmp(argv[1], "-w") == 0){
    w = atoi(argv[2]);
    // give error if not able to open the file
    if((fd = open(argv[3], 0)) < 0){
      printf(1, "unable to open the file %s\n", argv[3]);
      exit();
    }
    // call uniq function with -w flag
    uniq(fd, 3, w);
    close(fd);
    exit();
  }    
    

  // handling the -c, -u
  if(argc == 3) {
    option = argv[1];
    if((fd = open(argv[2], 0)) < 0){
      printf(1, "unable to open the file %s\n", argv[2]);
      exit();
    }
    // if the flag is -c call uniq function
    if(strcmp(option, "-c") == 0) {
      uniq(fd, 1, 0);
      // if the flag is -u call the uniq function
    } else if(strcmp(option, "-u") == 0) {
      uniq(fd, 2, 0);
    } else {
      printf(1, "unable to open the file %s\n", option);
      close(fd);
      exit();
    }
    close(fd);
  } else if(argc == 2) {
    if((fd = open(argv[1], 0)) < 0){
      printf(1, "unable to open file %s\n", argv[1]);
      exit();
    }
    uniq(fd, 0, 0);
    close(fd);
  } else {
    printf(1, "format: uniq [-c | -u | -w [N]] [file]\n");
    exit();
  }
  exit();
}