#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>

struct thread_arg{
  int num;
  int *have_finished;
};

void* thread_func(void* arg_) {
  struct thread_arg* arg = (struct thread_arg*)arg_;
  int num;
  while (*arg->have_finished != 1) {
    if (scanf("%d", &num) <= 0) {
      *arg->have_finished = 1;
      break;
    }
    arg->num += num;
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  uint64_t N;
  sscanf(argv[1], "%ld", &N);

  int have_finished = 0;
  struct thread_arg arg = {
    .num = 0,
    .have_finished = &have_finished
  };

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  pthread_attr_setguardsize(&attr, 0);

  pthread_t *threads = calloc(N, sizeof(pthread_t));

  for (int i = 0; i < N; ++i) {
    pthread_create(threads + i, &attr, thread_func, &arg);
  }

  for (int i = 0; i < N; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("%d\n", arg.num);
  pthread_attr_destroy(&attr);
  free(threads);
}