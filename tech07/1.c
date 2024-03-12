#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>


int main() {
  char* input;
  size_t input_size;
  getline(&input, &input_size, stdin);
  int fd = open("tmp.c", O_CREAT | O_WRONLY, 0644);
  if (fd == -1) {
    close(fd);
    perror("fd opening error\n");
    exit(1);
  }
  char prog_str[10000];
  snprintf(prog_str, 86 + input_size,"#include <stdio.h>\n int main() {\n int a = (%s);\n printf(\"%%d\", a); \nreturn 0;\n }", input);
  if (write(fd, prog_str, strnlen(prog_str, sizeof(prog_str))) < 0) {
    close(fd);
    perror("fd writing error\n");
    exit(1);
  }
  close(fd);
  pid_t p = fork();
  if (p == -1) {
    remove("tmp");
    perror("fork error\n");
    exit(1);
  }
  if (p == 0) {
    execlp("gcc", "gcc", "-o", "tmp", "tmp.c", NULL);
  }
  else {
    if (wait(NULL) == -1) {
      remove("tmp");
      remove("tmp.c");
      perror("wait error\n");
      exit(1);
    }
  }
  p = fork();
  if (p == -1) {
    remove("tmp");
    remove("tmp.c");
    perror("fork error\n");
    exit(1);
  }
  if (p == 0) {
    execlp("./tmp", "./tmp", NULL);
  }
  else {
    if (wait(NULL) == -1) {
      remove("tmp");
      remove("tmp.c");
      perror("wait error\n");
      exit(1);
    }
  }
  remove("tmp");
  remove("tmp.c");
  return 0;
}