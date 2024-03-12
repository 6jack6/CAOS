#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>

int main() {
  char file[PATH_MAX];
  uint64_t size = 0;
  while (fgets(file, sizeof(file), stdin)) {
    char *ptr = memchr(file, '\n', sizeof(file));
    if (ptr != NULL) {
      *ptr = '\0';
    }
    struct stat file_attr;
    if (lstat(file, &file_attr) > -1 && S_ISREG(file_attr.st_mode)) {
      size += file_attr.st_size;
    }
  }
  printf("%"PRIu64"\n", size);
}