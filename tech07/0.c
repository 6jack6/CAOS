#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  if (execlp("python3", "python3", "-c", "print(eval(input()))", NULL) < 0) {
    perror("cannot execute\n");
    exit(1);
  }
}