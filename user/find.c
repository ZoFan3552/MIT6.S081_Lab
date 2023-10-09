#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *getFileName(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void findFile(char *path, char *fileName)
{
  // printf("======current path : %s======\n" , path);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    char trimedFileName[DIRSIZ]; // 此处不能初始化太大，否则会造成xv6资源耗尽
    strcpy(trimedFileName, getFileName(path));
    char *p1 = trimedFileName;
    int len = strlen(trimedFileName);
    for (int i = 0; i < len; i++) // 去掉空格
    {
      if (*p1 == ' ')
      {
        memmove(p1, p1 + 1, len - i);
      }
      else
      {
        p1++;
      }
    }
    // printf("trimedFileName : %s$\n", trimedFileName);
    if (strcmp(trimedFileName, fileName) == 0)
    {
      if (strcmp(getFileName(path), fileName) == 0)
      {
        printf("%s\n", fileName);
      }
      else
      {
        printf("%s\n", path);
      }
      // close(fd);
      return;
    }

    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    { // 遍历子目录
      if (de.inum == 0)
        continue;

      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      findFile(buf, fileName);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(2, "usge : find dirName fileName\n");
    exit(1);
  }
  findFile(argv[1], argv[2]);
  exit(0);
}