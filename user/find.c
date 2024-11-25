#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void find(char* path, char* filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  // Open file path
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  
  // Get the file status from the file descriptor 
  if(fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  // Check type of the file just open
  switch(st.type){
  case T_DEVICE:
  case T_FILE:
  // If it was not a directory, thow an error message
    fprintf(2, "find: %s is not a directory\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    // Copy the path to the buffer and add "/" at the end of the path showing we are traversal inside the folder at "path"
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      // If we found a folder (not "." or ".."), go into that folder to find
      if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
        find(buf, filename);
      }
      // Found the file with name == filename, print it to the screen
      if (strcmp(de.name, filename) == 0) {
        printf("%s\n", buf);
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }
    if (argc == 2) {
        // Find file in current directory
        find(".", argv[1]);
    } else if (argc == 3) {
        // Find file in specified directory
        find(argv[1], argv[2]);
    }
    
    exit(0);
}