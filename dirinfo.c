
/* list all the files in the directory */
/* specify which files are directories (if any) */
/* show the total size of all the regular files the directory */
/* note that you do not have to recursively go through any subdirectories to find their size for this part (unless you want to, but that is not a simple task) */
/* Have the program scan the current directory */
/* Possible enhancements: */
/* Recursively list the files in any subdirectories, update the total to include the total size of subdirectories */
/* Print out the size in a more readable format (like using KB, MB, GB for -byte prefixes) */

#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>

int main() {
  DIR * d;
  d = opendir("test");
  struct dirent *entry;
  entry = readdir(d);
  
  readdir(d);
  printf("%s", entry->d_name);

  closedir(d);
  return 0;
}
