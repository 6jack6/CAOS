#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t flag1 = 0;
volatile sig_atomic_t flag2 = 0;
volatile sig_atomic_t end = 0;

void sigterm_handler(int sig_num) {
  end = 1;
  return;
};

void sigusr1_handler(int sig_num) {
  flag1 = 1;
};

void sigusr2_handler(int sig_num) {
  flag2 = 1;
};

int main() {
  struct sigaction sig_usr1_struct = {.sa_handler = sigusr1_handler, .sa_flags = SA_RESTART};
  struct sigaction sig_usr2_struct = {.sa_handler = sigusr2_handler, .sa_flags = SA_RESTART};
  struct sigaction sig_term_struct = {.sa_handler = sigterm_handler, .sa_flags = SA_RESTART};

  if (sigaction(SIGUSR1, &sig_usr1_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }
  if (sigaction(SIGUSR2, &sig_usr2_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }
  if (sigaction(SIGTERM, &sig_term_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }
  if (sigaction(SIGINT, &sig_term_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }

  int value;

  printf("%d\n", getpid());
  fflush(stdout);
  scanf("%d", &value);

  while (!end) {
    if (flag1) {
      ++value;
      printf("%d\n", value);
      fflush(stdout);
      flag1 = 0;
    }
    if (flag2) {
      value *= -1;
      printf("%d\n", value);
      fflush(stdout);
      flag2 = 0;
    }
    pause();
  }
}