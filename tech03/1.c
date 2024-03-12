#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

struct Item {
  int value;
  uint32_t next_pointer;
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    close(fd);
    return 1;
  }

  struct Item item = {1, 1};
  while (item.next_pointer != 0) {
    if (read(fd, &item, sizeof(item)) <= 0) {
      close(fd);
      return 0;
    }
    printf("%d ", item.value);
    item.next_pointer = lseek(fd, item.next_pointer, SEEK_SET);
  }

  close(fd);
}