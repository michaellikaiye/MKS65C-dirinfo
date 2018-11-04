
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
#include<pwd.h>
void printHelper(int fileMode, int b, int c, char p) {
  if ((fileMode & b) && (fileMode & c))
    printf("%c",p);
  else
    printf("-");
}

void printPermissions(int fileMode) {
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

static int myCompare (const void * a, const void * b) {
  return strcmp (*(const char **) a, *(const char **) b);
}

void sort(const char *arr[], int n) {
  qsort (arr, n, sizeof (const char *), myCompare);
}

// prints file size in readable format
void printSize(double size) {
  char * sizearray[5] = {" ", "K", "M", "G", "T"};
  int ps = 0;
  while(size > 1000) {
    size = size / 1000.0;
    ps++;
  }
  printf("%6.2f %sB", size, sizearray[ps]);
}

// prints file info
void printStat(char * str) {
  // allocate some memory
  struct stat * pter = malloc(sizeof(struct stat));

  // record metadata into pter
  stat(str, pter);

  //prints file permissions
  //printf("mode(permissions): (%3o)\n", pter -> st_mode&0777);
  printPermissions(pter -> st_mode);
  printf(" ");

  //print user id and group id of owner
  struct passwd * pwd =  malloc(sizeof(struct passwd));
  pwd = getpwuid(pter->st_uid);
  printf("  %s", pwd->pw_name);
  pwd = getpwuid(pter->st_gid);
  printf(" %s ", pwd->pw_name);

  // prints file size
  long int size = pter -> st_size;
  printSize(size);

  //prints time of last access
  char temp[100];
  strftime(temp, 100, "%b %e %H:%M", localtime(&(pter -> st_atime)));
  printf(" %s  ", temp);


  //prints file name
  printf(" %s\n", str);

  // frees memory
  free(pter);
}

// prints directory information (tab is for formatting based on the directory level)
long int printDir(char *str) {
  printf("\n%s directory information: \n", str);
  // opens directory stream
  struct dirent *entry;
  long int fsize = 0;
  DIR * d;
  d = opendir(str);
  if (d == NULL) {
    printf("Could not open current directory 1\n");
    return 0;
  }
  int dirNum = 0;
  int filNum = 0;
  while(entry = readdir(d)){
    if (entry->d_type == 4 && strncmp(entry->d_name, ".", 1) != 0)
      dirNum++;
    if (entry->d_type == 8)
      filNum++;
  }
  printf("directories: %d\n", dirNum);
  printf("files: %d\n",filNum);
  closedir(d);

  d = opendir(str);
  //error stuff?? how to implement
  if (d == NULL){
    printf("Could not open current directory 2\n");
    return 0;
  }

  char *dirArray[dirNum];
  int dA = 0;
  // prints directory information
  while(entry = readdir(d)){

    // allocate some memory for file information
    struct stat * po = malloc(sizeof(struct stat));
    stat(entry->d_name, po);

    // prints information in the current directory but prevents recursively printing the directory itself
    if (entry->d_type == 4) {
      printf("\033[0;34m");
      printf("d");
      printStat(entry->d_name);
      printf("\033[0m");
      if(strncmp(entry->d_name, ".", 1) != 0) {
        dirArray[dA] = entry->d_name;
        dA++;
      }
      //BROKEN
      //printDir(entry->d_name);
    }

    else if (entry->d_type == 8){
      printf("-");
      stat(entry->d_name, po);
      printStat(entry->d_name);
      // updates total size
      fsize += po->st_size;
    }
    else;
    //frees memory
    free(po);
  }
  printf("total size: ");
  printSize(fsize);
  printf("\n");

  // closes directory
  closedir(d);

  //print other subdirectories
  //sort(dirArray, dA);
  int h;
  for(h = 0; h < dA; h++) {
    printDir(dirArray[h]);
  }
  // prints total directory size

  return fsize;
}



int main() {
  printf("directory information (note that . files are ignored when printing subdirectories)\n\n");
  printDir(".");

  return 0;
  }
