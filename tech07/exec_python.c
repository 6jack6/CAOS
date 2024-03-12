//
// Created by antony on 28.12.23.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {


  pid_t pid = fork();
  if (pid == -1) {
    perror("fork error\n");
    exit(1);
  } else if (pid == 0) {
    // —юда путь к файлу написать
    if (execlp("python3", "python3", "/home/antony/CLionProjects/CAOS/tech07/helloworld.py", NULL) < 0) {
      perror("cannot execute\n");
      exit(1);
    }
  } else {
    if (wait(NULL) == -1) {
      perror("wait error\n");
      exit(1);
    }
  }


  return 0;
}