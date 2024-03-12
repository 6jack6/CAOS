#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t end = 1;
volatile sig_atomic_t flag = 0;
volatile sig_atomic_t sig_n = 0;

void handler(int sig_num) {
  flag = 1;
  sig_n = sig_num;
}

void finish_handler(int sig_num) {
  end = 0;
}

int main(int argc, char* argv[]) {
  // printf("%d %d\n", SIGRTMIN, SIGRTMAX);
  if (argc == 1) {
    perror("inappropriate args\n");
    exit(1);
  }

  FILE* files[1000];
  for (int i = 0; i < argc - 1; ++i) {
    files[i] = fopen(argv[i + 1], "r");
    if (files[i] == NULL) {
      perror("cannot open file\n");
      for (int j = 0; j < i; ++j) {
        fclose(files[j]);
      }
      exit(1);
    }
  }

  struct sigaction sig_struct = {
    .sa_handler = handler,
    .sa_flags = SA_RESTART
  };
  struct sigaction sigrtmin_struct = {
    .sa_handler = finish_handler,
    .sa_flags = SA_RESTART
  };

  for (int i = SIGRTMIN + 1; i <= SIGRTMAX; ++i) {
    if (sigaction(i, &sig_struct, NULL) == -1) {
      perror("sigaction error\n");
      exit(1);
    }
  }
  if (sigaction(SIGRTMIN, &sigrtmin_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }

  while (end) {
    if (flag) {
      char *string = NULL;
      size_t size = 0;
      getline(&string, &size, files[sig_n - SIGRTMIN - 1]);
      printf("%s", string);
      fflush(stdout);
      free(string);
      flag = 0;
    }
    pause();
  }
  for (int i = 0; i < argc - 1; ++i) {
    fclose(files[i]);
  }
}