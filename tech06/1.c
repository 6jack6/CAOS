#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("few arguments\n");
    exit(1);
  }
  size_t number = strtol(argv[1], NULL, 10);
  pid_t pid;
  for (size_t i = 1; i <= number; ++i) {
    pid = fork();
    if (pid == -1) {
      perror("N is larger than number of processes limit\n");
      exit(1);
    }
    if (pid == 0) {
      printf("%zu", i);
      if (i != number) {
        printf(" ");
      }
      exit(0);
    }
    wait(NULL);
  }
  printf("\n");
  return 0;
}