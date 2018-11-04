
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
  printf("file: %s\n", str);

  // prints file size
  long int size = pter -> st_size;
  printSize(size);

  //prints file permissions
  printf("mode(permissions): (%3o)\n", pter -> st_mode&0777);
  printPermissions(pter -> st_mode);

  //prints time of last access
  char temp[100];
  strftime(temp, 100, "%c", localtime(&(pter -> st_atime)));
  printf("\ntime accessed: %s\n", temp);

  // frees memory
  free(pter);
}

// prints directory information (tab is for formatting based on the directory level)
long int printDir(char *str, int tab) {
  
  // for indentation when printing
  char tabString[100];
  int i;
  for(i = 0; i < 3 * tab; i++) {
    tabString[i] = ' ';
  }
  tabString[i] = '\0';

  // opens directory stream
  struct dirent *entry;
  long int fsize = 0;
  DIR * d;
  d = opendir(str);

  //error stuff?? how to implement
  if (d == NULL){
    printf("%sCould not open current directory\n", tabString);
    return 0;
  }

  // prints directory information
  while(entry = readdir(d)){

    // allocate some memory for file information
    struct stat * po = malloc(sizeof(struct stat));
    stat(entry->d_name, po);

    // prints information in the current directory but prevents recursively printing the directory itself
    if (entry->d_type == 4 && strncmp(entry->d_name, ".", 1) != 0) {

      // prints directory name
      printf("%sDirectory name: %s\n", tabString, entry->d_name);   
      printf("%s  ", tabString);

      // prints directory permissions
      printf(" Directory permissions:");
      printPermissions(po->st_mode);
      printf("\n%s  ", tabString);

      // prints directory size
      printf(" Directory size: ");
      printSize(po->st_size);
      printf("\n\n");

      
      printDir(entry->d_name, tab + 3);
    }

    else if (entry->d_type == 8){
      stat(entry->d_name, po);

      // prints file name
      printf("%sFile name: %s\n", tabString, entry->d_name);
      printf("%s  ", tabString);

      // prints file permissions
      // ==============ERROR HERE: ST_MODE RETURNS 0=============
      printf("File permissions: ");
      printPermissions(po->st_mode);
      printf("\n %s  ", tabString);

      // prints file size
      printf("file size: ");
      printSize(po->st_size);
      printf("\n\n");


      // updates total size
      fsize += po->st_size;

      //frees memory
      free(po);
    }
    else;
  }

  // prints total directory size
  printf("%sdirectory %s total size:", tabString, str);
  printSize(fsize);
  printf("\n");

  // closes directory
  closedir(d);
  
  return fsize;
}



int main() {
  int totalSize;
  printf("current directory information: \n");
  totalSize = printDir(".", 1);
  //printStat("dirinfo.c");



  return 0;
  }
