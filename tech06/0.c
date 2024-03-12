#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>

int main() {
  uint64_t process_cnt = 1;
  while(1) {
    pid_t pid = fork();

    if (-1 == pid) {
      printf("%ld\n", process_cnt);
      break;
    } else if (0 == pid) {
      ++process_cnt;
    } else {
      waitpid(pid, NULL, 0);
      break;
    }
  }
}