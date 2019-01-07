#include <dirent.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <queue>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace std;
int num;

void printfile_bfs(int &depth);
void printinfo(struct stat &sb, char *name);

int main(int argc, char *argv[]) {
  int depth = 0;
  printfile_bfs(depth);
  printf("\nfile num total : %d\n", num);
  return 0;
}

void printfile_bfs(int &depth) {
  queue<char *> dir_queue;
  dir_queue.push(getcwd(NULL, PATH_MAX));
  while (dir_queue.empty() == false) {
    int this_level_count = dir_queue.size();
    depth += 4; // depth increase only when walk to next level
    for (int i = 0; i < this_level_count; ++i) {
      char *cur_dir_str = dir_queue.front();
      dir_queue.pop();
      DIR *dir_ptr = opendir(cur_dir_str);
      chdir(cur_dir_str);
      struct dirent *file_ptr;
      struct stat file_stat;
      if (dir_ptr == NULL) {
        printf("error open directory %s\n", cur_dir_str);
        continue;
      }
      int file_count = 0;
      while ((file_ptr = readdir(dir_ptr)) != NULL) {
        if (lstat(file_ptr->d_name, &file_stat) < 0) {
          printf("error stat file %s info \n", file_ptr->d_name);
          continue;
        }
        if ((strcmp(file_ptr->d_name, ".") == 0) ||
            (strcmp(file_ptr->d_name, "..") == 0)) {
          // current directory or father directory
          continue;
        }
        num++;
        file_count++;
        if (S_ISDIR(file_stat.st_mode)) { // the file is directory
          printinfo(file_stat, file_ptr->d_name);
          cout << " " << depth << endl;
          // char *child_dir = (char *)malloc(sizeof(char) * PATH_MAX);
          // strcpy(child_dir, cur_dir_str);
          // strcat(child_dir, "/");
          // strcat(child_dir, file_ptr->d_name);
          // dir_queue.push(child_dir);
          string child_dir = cur_dir_str;
          child_dir += "/";
          child_dir += file_ptr->d_name;
          char *child_dir_name =
              (char *)malloc(sizeof(char) * (child_dir.size() + 1));
          strcpy(child_dir_name, child_dir.c_str());
          dir_queue.push(child_dir_name);
        } else {
          printinfo(file_stat, file_ptr->d_name);
          cout << " " << depth << endl;
        }
      }
      printf("directory %s has %d file in total\n\n", cur_dir_str, file_count);
      closedir(dir_ptr);
    }
  }
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
