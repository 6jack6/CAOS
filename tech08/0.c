#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

pid_t launch(const char *cmd, int fd_in, int fd_out) {
  pid_t pid = fork();

  if (0 == pid) {
    if (-1 == dup2(fd_in, STDIN_FILENO)) {
      return -1;
    }
    close(fd_in);

    if(-1 == dup2(fd_out, STDOUT_FILENO)) {
      return -1;
    }
    close(fd_out);

    execlp(cmd, cmd, NULL);
    return -1;
  }

  return pid;
}

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    perror("inappropriate args\n");
    exit(1);
  }

  int fd_in = open(argv[2], O_RDONLY);
  if (fd_in == -1) {
    close(fd_in);
    perror("opening file error\n");
    exit(1);
  }

  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    close(fd_in);
    close(pipe_fd[1]);
    perror("cannot create a pipe\n");
    exit(1);
  }

  if (launch(argv[1], fd_in, pipe_fd[1]) == -1) {
    close(fd_in);
    close(pipe_fd[1]);
    perror("forking error\n");
    exit(1);
  }
  close(pipe_fd[1]);

  size_t ans = 0;
  char buf[4096];
  size_t read_n = 0;

  while ((read_n = read(pipe_fd[0], buf, 4096))) {
    if (read_n == -1) {
      perror("reading from file error\n");
      exit(1);
    }
    ans += read_n;
  }

  printf("%zu\n", ans);
}