#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

void delete_newline(char *file);

int main() {
  char str_file[PATH_MAX];

  while (fgets(str_file, PATH_MAX, stdin) != NULL) {
    delete_newline(str_file);
    if (access(str_file, X_OK) == 0) {
      int fd = open(str_file, O_RDONLY);
      if (fd == -1) {
        perror("opening error\n");
        exit(1);
      }
      char buffer[PATH_MAX];
      if (read(fd, buffer, PATH_MAX) == -1) {
        perror("reading error\n");
        exit(1);
      }

      if (buffer[0] == '#' && buffer[1] == '!') {
        char *ptr = buffer + 2;
        delete_newline(ptr);
        if (access(ptr, X_OK) != 0) {
          printf("%s\n", str_file);
          close(fd);
          continue;
        }
      } else if (buffer[0] != 0x7f || buffer[1] != 0x45 ||
          buffer[2] != 0x4c || buffer[3] != 0x46) {
        printf("%s\n", str_file);
      }
      close(fd);
    }
  }
  return 0;
}

void delete_newline(char *file) {
  char *ptr = file;
  while (*ptr != '\0' && *ptr != '\n') {
    ++ptr;
  }
  if (*ptr == '\n') {
    *ptr = '\0';
  }
}