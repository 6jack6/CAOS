#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    perror("inappropriate args\n");
    exit(1);
  }
  int fds[2];
  if (pipe(fds) == -1) {
    perror("pipe error\n");
    exit(1);
  }
  pid_t pid1 = fork();
  if (pid1 == -1) {
    perror("forking error\n");
    close(fds[1]);
    close(fds[0]);
    exit(1);
  }
  if (pid1 == 0) {
    if (dup2(fds[1], 1) == -1) {
      perror("duplication error\n");
      close(fds[1]);
      close(fds[0]);
      exit(1);
    }
    close(fds[1]);
    close(fds[0]);
    execlp(argv[1], argv[1], NULL);
    perror("execution error\n");
    exit(1);
  }
  close(fds[1]);
  pid_t pid2 = fork();
  if (pid2 == -1) {
    perror("forking error\n");
    close(fds[0]);
    exit(1);
  }
  if (pid2 == 0) {
    if (dup2(fds[0], 0) == -1) {
      perror("duplication error\n");
      close(fds[0]);
      exit(1);
    }
    close(fds[0]);
    execlp(argv[2], argv[2], NULL);
    perror("execution error\n");
    close(fds[0]);
    exit(1);
  }
  close(fds[0]);
  if (waitpid(pid1, NULL, 0) == -1 || waitpid(pid2, NULL, 0) == -1) {
    perror("waiting for children to die error\n");
    exit(1);
  }
}