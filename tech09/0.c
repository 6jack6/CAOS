#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t sigint = 0;
volatile sig_atomic_t sigterm = 0;

void process_sigint(int signum) {
  sigint = 1;
}

void process_sigterm(int signum) {
  sigterm = 1;
}

int main() {
  size_t counter = 0;

  struct sigaction sigint_action = {};
  struct sigaction sigterm_action = {};

  sigint_action.sa_handler = process_sigint;
  sigterm_action.sa_handler = process_sigterm;

  if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }
  if (sigaction(SIGTERM, &sigterm_action, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }

  printf("%d\n", getpid());
  fflush(stdout);

  while (1) {
    if(sigint) {
      sigint = 0;
      ++counter;
    }

    if (sigterm) {
      printf("%zu\n", counter);
      return 0;
    }

    pause();
  }
}