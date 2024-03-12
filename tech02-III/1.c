#include <sys/syscall.h>
long syscall(long n, ...);

#define MAX_SIZE 4*4096
#define fd_in 0
#define fd_out 1

void _start() {
  char input[MAX_SIZE];
  int size = 0;
  do {
    size = syscall(SYS_read, fd_in, input, sizeof(input));
    syscall(SYS_write, fd_out, input, size);
  } while (size > 0);
  syscall(SYS_exit, 0);
}