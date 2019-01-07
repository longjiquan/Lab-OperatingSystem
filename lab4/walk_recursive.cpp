#include <dirent.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace std;
int num;

void printfile(int &depth);
void printinfo(struct stat &sb, char *name);

int main() {
  int depth = 0;
  printfile(depth);
  cout << num << endl;
  return 0;
}
void printfile(int &depth) {
  DIR *d;              //声明一个句柄
  struct dirent *file; // readdir函数的返回值就存放在这个结构体中
  struct stat sb;
  if (!(d = opendir("."))) {
    printf("error opendir\n");
    return;
  }
  while ((file = readdir(d)) != NULL) {
    num++;
    if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
      continue;
    if (lstat(file->d_name, &sb) < 0) {
      continue;
    }
    if (S_ISDIR(sb.st_mode)) {
      printinfo(sb, file->d_name);
      cout << " " << depth << endl;
      depth += 4;
      string s = "./";
      s += file->d_name;
      if (chdir(s.c_str()) == 0)
        printfile(depth);
    } else {
      printinfo(sb, file->d_name);
      cout << " " << depth << endl;
    }
  }
  closedir(d);
  if (depth != 0)
    chdir("..");
  depth -= 4;
}
void printinfo(struct stat &sb, char *name) {
  struct passwd *pd;
  struct group *gp;
  struct tm t;
  tzset();
  localtime_r(&(sb.st_mtime), &t);
  char buf[30];
  strftime(buf, 30, "%Y-%m-%d %H:%M", &t);

  if (S_ISDIR(sb.st_mode))
    cout << 'd';
  else
    cout << '-';
  if ((S_IRUSR & sb.st_mode) == S_IRGRP)
    cout << 'r';
  else
    cout << '-';
  if ((S_IWUSR & sb.st_mode) == S_IWUSR)
    cout << 'w';
  else
    cout << '-';
  if ((S_IXUSR & sb.st_mode) == S_IXUSR)
    cout << 'x';
  else
    cout << '-';
  if ((S_IRGRP & sb.st_mode) == S_IRGRP)
    cout << 'r';
  else
    cout << '-';
  if ((S_IWGRP & sb.st_mode) == S_IWGRP)
    cout << 'w';
  else
    cout << '-';
  if ((S_IXGRP & sb.st_mode) == S_IXGRP)
    cout << 'x';
  else
    cout << '-';
  if ((S_IROTH & sb.st_mode) == S_IROTH)
    cout << 'r';
  else
    cout << '-';
  if ((S_IWOTH & sb.st_mode) == S_IWOTH)
    cout << 'w';
  else
    cout << '-';
  if ((S_IXOTH & sb.st_mode) == S_IXOTH)
    cout << 'x';
  else
    cout << '-';
  cout << " ";
  cout << sb.st_nlink;
  cout << " ";
  pd = getpwuid(sb.st_uid);
  if (pd != NULL)
    cout << pd->pw_name << " ";
  gp = getgrgid(sb.st_gid);
  if (gp != NULL)
    cout << gp->gr_name << " ";
  cout << sb.st_size << " ";
  cout << buf << " ";
  cout << name << " ";
}
