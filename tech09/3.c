#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/signalfd.h>

volatile sig_atomic_t pid = 0;
volatile sig_atomic_t end = 0;
volatile sig_atomic_t value = 0;
volatile sig_atomic_t flag = 0;

void signal_handler(int sig_num, siginfo_t* sig_info, void* context) {
  if (sig_info->si_value.sival_int == 0) {
    end = 1;
    return;
  }
  flag = 1;
  pid = sig_info->si_pid;
  value = sig_info->si_value.sival_int;
}

int main() {
  sigset_t signals_to_be_ignored;
  if (sigfillset(&signals_to_be_ignored) == -1) {
    perror("cannot fill a set of signals\n");
    exit(1);
  }
  if (sigdelset(&signals_to_be_ignored, SIGRTMIN) == -1) {
    perror("cannot delete a signal from a set\n");
    exit(1);
  }
  if (sigprocmask(SIG_BLOCK, &signals_to_be_ignored, NULL) == -1) {
    perror("cannot ignore signals from the set\n");
    exit(1);
  }

  struct sigaction sigrtmin_struct;

  sigrtmin_struct.sa_sigaction = signal_handler;
  sigrtmin_struct.sa_flags = SA_SIGINFO | SA_RESTART;

  if (sigaction(SIGRTMIN, &sigrtmin_struct, NULL) == -1) {
    perror("sigaction error\n");
    exit(1);
  }

  while (!end) {
    if (flag) {
      --value;
      if (pid != 0) {
        sigqueue(pid, SIGRTMIN, (union sigval){.sival_int = value});
        pid = 0;
      }
      flag = 0;
    }
    pause();
  }
}