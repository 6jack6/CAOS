#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Item {
  int value;
  uint32_t next_pointer;
};

off_t file_size(int fd) {
  struct stat st;
  fstat(fd, &st);
  return st.st_size;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    perror("few parameters\n");
    exit(1);
  }

  char* filename = argv[1];
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    close(fd);
    perror("file opening error\n");
    exit(1);
  }

  off_t f_size = file_size(fd);
  if (f_size == 0) {
    close(fd);
    perror("invalid file\n");
    exit(1);
  }

  struct Item item = {0, 1};

  const struct Item* const file_data = mmap(NULL, f_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (file_data == MAP_FAILED)
  {
    close(fd);
    perror("map error\n");
    exit(1);
  }
  const struct Item* cur_file_ptr = file_data;

  while (item.next_pointer != 0) {
    item = *cur_file_ptr;
    printf("%d ", item.value);
    cur_file_ptr = file_data + item.next_pointer / sizeof(struct Item);
  }
  printf("\n");

  munmap((void *)file_data, f_size);
  close(fd);
  return 0;
}