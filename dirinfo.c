
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
void printHelper(int fileMode, int b, int c, char p) {
  if ((fileMode & b) && (fileMode & c))
    printf("%c",p);
  else
    printf("-");
}

void printPermissions(int fileMode) {
  printf("-");
  /* owner permissions */
  printHelper(fileMode, S_IRUSR, S_IREAD, 'r');
  printHelper(fileMode, S_IWUSR, S_IWRITE, 'w');
  printHelper(fileMode, S_IXUSR, S_IEXEC, 'x');
  /* group permissions */
  printHelper(fileMode, S_IRGRP, S_IREAD, 'r');
  printHelper(fileMode, S_IWGRP, S_IWRITE, 'w');
  printHelper(fileMode, S_IXGRP, S_IEXEC, 'x');
  /* other user permissions */
  printHelper(fileMode, S_IROTH, S_IREAD, 'r');
  printHelper(fileMode, S_IWOTH, S_IWRITE, 'w');
  printHelper(fileMode, S_IXOTH, S_IEXEC, 'x');
}

void printSize(double size) {
  char * sizearray[5] = {"", "K", "M", "G", "T"};
  int ps = 0;
  while(size > 1000) {
    size = size / 1000.0;
    ps++;
  }
  printf("%f %sB", size, sizearray[ps]);
}


void printStat(char * str) {
  struct stat * pter = malloc(sizeof(struct stat)); // allocate some memory
  stat(str, pter); // record metadata into pter
  // printing file info:
  printf("file: %s\n", str);
  long int size = pter -> st_size;
  printSize(size);
  printf("mode(permissions): (%3o)\n", pter -> st_mode&0777);
  printPermissions(pter -> st_mode);
  char temp[100];
  strftime(temp, 100, "%c", localtime(&(pter -> st_atime)));
  printf("\ntime accessed: %s\n", temp);
  free(pter);
}

long int printDir(char *str, int tab) {
  char tabString[100];
  int i;
  for(i = 0; i < 3 * tab; i++) {
    tabString[i] = ' ';
  }
  tabString[i] = '\0';
  struct dirent *entry;
  long int fsize = 0;
  DIR * d;
  d = opendir(str);

  //error stuff?? how to implement
  if (d == NULL){
    printf("%sCould not open current directory\n", tabString);
    return 0;
  }
  while(entry = readdir(d)){
    struct stat * po = malloc(sizeof(struct stat));
    stat(entry->d_name, po);
    if (entry->d_type == 4 && strncmp(entry->d_name, ".", 1) != 0) {
      printf("%s%s (directory name)\n", tabString, entry->d_name);   
      printf("%s  ", tabString);
      printPermissions(po->st_mode);
      printf(" (directory permissions)\n");
      printf("%s  ", tabString);
      printSize(po->st_size);
      printf(" (directory size)\n");
      printDir(entry->d_name, tab + 3);
    }

    else if (entry->d_type == 8){
      
      stat(entry->d_name, po);
      printf("%s%s (file name)\n", tabString, entry->d_name);
      printf("%s  ", tabString);
      printPermissions(po->st_mode);
      printf(" (file permissions)\n");
      printf("%s  ", tabString);
      printSize(po->st_size);
      printf(" (file size)\n");
      fsize += po->st_size;
      free(po);
    }
    else;
  }
  printf("%sdirectory %s total size:", tabString, str);
  printSize(fsize);
  printf("\n");
  closedir(d);
  return fsize;
}

int main() {
  int totalSize;
  printf("stuff in current dirctory: \n");
  totalSize = printDir(".", 1);
  //printStat("dirinfo.c");



  return 0;
  }
