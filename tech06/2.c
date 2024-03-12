#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  char word[4096] = {};
  size_t counter = 0;
  pid_t pid = 0;
  int status = 0;
  while(1) {
    pid = fork();
    if (pid == -1) {
      perror("cannot fork process\n");
      exit(1);
    }
    if (pid == 0) {
      fflush(stdin);
      status = scanf("%s", word);
      exit(status != EOF);
    } else {
      if (waitpid(pid, &status, 0) == -1) {
        perror("cannot wait for process state change\n");
        exit(1);
      }
      counter += WEXITSTATUS(status);
      if (status == 0) {
        break;
      }
    }
  }
  printf("%zu\n", counter);
  return 0;
}