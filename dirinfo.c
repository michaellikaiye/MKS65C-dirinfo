
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

// prints file size in readable format
void printSize(double size) {
  char * sizearray[5] = {"", "K", "M", "G", "T"};
  int ps = 0;
  while(size > 1000) {
    size = size / 1000.0;
    ps++;
  }
  printf("%f %sB", size, sizearray[ps]);
}

// prints file info
void printStat(char * str) {
  // allocate some memory
  struct stat * pter = malloc(sizeof(struct stat));

  // record metadata into pter
  stat(str, pter);

  //prints file permissions
  //printf("mode(permissions): (%3o)\n", pter -> st_mode&0777);
  printf("-");
  printPermissions(pter -> st_mode);
  printf("  ");

  //print user id and group id of owner
  struct passwd * pwd =  malloc(sizeof(struct passwd));
  pwd = getpwuid(pter->st_uid);
  printf("  %s  ", pwd->pw_name);
  pwd = getpwuid(pter->st_gid);
  printf("  %s  ", pwd->pw_name);
  
  //prints time of last access
  char temp[100];
  strftime(temp, 100, "%c", localtime(&(pter -> st_atime)));
  printf(" %s  ", temp);

  // prints file size
  long int size = pter -> st_size;
  printSize(size);

  //prints file name
  printf(" %s\n", str);

  // frees memory
  free(pter);
}

// prints directory information (tab is for formatting based on the directory level)
long int printDir(char *str) {
  
  // opens directory stream
  struct dirent *entry;
  long int fsize = 0;
  DIR * d;
  d = opendir(str);

  //error stuff?? how to implement
  if (d == NULL){
    printf("Could not open current directory\n");
    return 0;
  }

  // prints directory information
  while(entry = readdir(d)){

    // allocate some memory for file information
    struct stat * po = malloc(sizeof(struct stat));
    stat(entry->d_name, po);

    // prints information in the current directory but prevents recursively printing the directory itself
    if (entry->d_type == 4 && strncmp(entry->d_name, ".", 1) != 0) {


      // prints directory permissions
      printf("d");
      printPermissions(po->st_mode);
      printf("  ");


      // USER, GROUP, AND TIME ARE NOT AVAILABLE
      printf(" USERID   GROUPID  ");
      printf(" day mon dd hh:mm:ss yyyy  ");

      // prints directory size
      printSize(po->st_size);

      // prints directory name
      printf("  %s\n", entry->d_name);   

      //BROKEN
      //printDir(entry->d_name);
    }

    else if (entry->d_type == 8){
      stat(entry->d_name, po);

      printStat(entry->d_name);

      // updates total size
      fsize += po->st_size;

      //frees memory
      free(po);
    }
    else;
  }

  // prints total directory size
  printf("total size: ");
  printSize(fsize);
  printf("\n");

  // closes directory
  closedir(d);
  
  return fsize;
}



int main() {
  int totalSize;
  printf("current directory information: \n");
  totalSize = printDir(".");
  //printStat("dirinfo.c");



  return 0;
  }
