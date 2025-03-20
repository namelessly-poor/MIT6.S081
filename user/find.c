#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *prev = 0;
  char *curr = strchr(path, '/');
  while (curr != 0) {
    prev = curr;
    curr = strchr(curr + 1, '/');
  }
  return prev;
}

void find(char *cur_path, char *target) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(cur_path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", cur_path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", cur_path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    char *file_name = fmtname(cur_path);
    int match = 1;
    if (file_name == 0 || strcmp(file_name + 1, target) != 0) {
      match = 0;
    }
    if (match) {
      printf("%s\n", cur_path);
    }
    close(fd);
    break;

  case T_DIR:
    if(strlen(cur_path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, cur_path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0|| strcmp(de.name, ".") == 0 ||
      strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, target);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "usage: find [directory] [target filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}