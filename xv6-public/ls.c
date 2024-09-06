#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char* formatname(char *path, int type)
{  
  // create a buffer with size DIRSIZ+1
  static char buffer[DIRSIZ+1];
  char *p;
  // Move p to point to the end of the string
  p = path + strlen(path); 
  // Loop to move backwards through the string to find the last '/'
  while (p >= path) {
      if (*p == '/') {
        // Exit the loop if '/' is found
        break; 
      }
      p--;
  }
p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p; 
  memmove(buffer, p, strlen(p));
  
  // Append '/' for directories,if type == T_DIR
  if(type == T_DIR){
    memset(buffer+strlen(p), '/', 1);
    memset(buffer+strlen(p)+1, ' ', DIRSIZ-strlen(p));
  }else{
    memset(buffer+strlen(p), ' ', DIRSIZ-strlen(p));
  }
  return buffer;
}
void
ls(char *path, int show_hidden)
{
  char buffer[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "unable to open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    printf(2, "unable to open %s\n", path);
    close(fd);
    return;
  }
  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", formatname(path, st.type), st.type, st.ino, st.size);
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buffer){
      printf(1, "given path is too long\n");
      break;
    }
    strcpy(buffer, path);
    p = buffer+strlen(buffer);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // for ls command skip the hidden files
      if(!show_hidden && de.name[0] == '.') 
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buffer, &st) < 0){
        printf(1, "unable to open %s\n", buffer);
        continue;
      }
      printf(1, "%s %d %d %d\n", formatname(buffer,st.type), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  int show_hidden=0;
  // -a flag, to display the hidden files
  if(argc>1 && strcmp(argv[1], "-a")==0)
  {
    show_hidden=1;
    argc--;
    argv++;
  }
  if(argc < 2){
    ls(".", show_hidden);
    exit();
  }
  // check every file and directory
  for(i=1; i<argc; i++)
    ls(argv[i], show_hidden);
  exit();
}
