#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void print_errors(char* string, int fd) {
  size_t err_num = 0, warnings_num = 0;
  int last_err = -1, last_warning = -1;
  size_t tmp_string = 0, symbol = 0;

  char format[4096] = {'\0'};
  strcat(format, string);
  strcat(format, ":%lu:%lu:");

  char buff[4096];
  FILE* file = fdopen(fd, "r");

  while (fscanf(file, "%s", buff) != EOF) {
    if (sscanf(buff, format, &tmp_string, &symbol) == 2) {
      fscanf(file, "%s", buff);
      if (strcmp("error:", buff) == 0) {
        if (tmp_string != last_err) {
          last_err = tmp_string;
          ++err_num;
        }
      }
      if (strcmp("warning:", buff) == 0) {
        if (tmp_string != last_warning) {
          last_warning = tmp_string;
          ++warnings_num;
        }
      }
    }
  }
  printf("%ld %ld\n", err_num, warnings_num);
  fclose(file);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("inappropriate args\n");
    exit(1);
  }

  int fds[2];
  if (pipe(fds) == -1) {
    close(fds[0]);
    close(fds[1]);
    perror("pipe error\n");
    exit(1);
  }

  pid_t pid = fork();
  if (pid == -1) {
    close(fds[0]);
    perror("forking error\n");
    exit(1);
  }

  if (pid == 0) {
    if (dup2(fds[1], STDERR_FILENO) == -1) {
      close(fds[1]);
      close(fds[0]);
      perror("duplication error\n");
      exit(1);
    }
    close(fds[1]);
    execlp("gcc", "gcc", argv[1], NULL);
    perror("execution error\n");
    exit(1);
  }
  wait(NULL);
  close(fds[1]);
  print_errors(argv[1], fds[0]);
  close(fds[0]);
}