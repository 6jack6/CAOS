#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

off_t file_size(int fd) {
  struct stat st;
  fstat(fd, &st);
  return st.st_size;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    perror("few parameters\n");
    exit(1);
  }

  char *file_name = argv[1];
  char *substr = argv[2];

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    close(fd);
    perror("file opening error\n");
    exit(2);
  }

  off_t f_size = file_size(fd);
  if (f_size == 0) {
    perror("file size error\n");
    exit(3);
  }

  const char *const file_data = mmap(NULL, f_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (file_data == MAP_FAILED) {
    perror("mmap error\n");
    exit(4);
  }

  const char *it = file_data;
  const char *pos;
  size_t file_data_len = f_size;

  pos = (const char *)memmem(it, file_data_len, substr, strlen(substr));
  while (pos != NULL) {
    file_data_len -= (pos - it + 1);
    it = pos + 1;
    printf("%ld\n", pos - file_data);
    pos = (const char *)memmem(it, file_data_len, substr, strlen(substr));
  }

  munmap((void *) file_data, f_size);
  close(fd);
  return 0;
}