#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct thread_arg {
  pthread_mutex_t *m;
  double *elems;
  int N, k;
  int i;
};

void *thread_worker(void *arg_) {
  struct thread_arg *arg = (struct thread_arg*) arg_;

  pthread_mutex_lock(arg->m);

  for (int j = 0; j < arg->N; j++) {
    arg->elems[arg->i] += 1.0;
    arg->elems[(arg->i + 1) % arg->k] += 1.01;
    if (arg->i > 0) arg->elems[arg->i - 1] += 0.99;
    else arg->elems[arg->k - 1] += 0.99;
  }

  pthread_mutex_unlock(arg->m);
}

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    return 1;
  }

  struct thread_arg args[1000];

  long N = strtol(argv[1], NULL, 10);
  long k = strtol(argv[2], NULL, 10);

  pthread_t *threads = calloc(k, sizeof(pthread_t));
  double *elems = calloc(k, sizeof(double));
  pthread_mutex_t m;
  pthread_mutex_init(&m, NULL);

  for (size_t i = 0; i < k; i++) {
    pthread_mutex_lock(&m);
    args[i].m = &m;
    args[i].elems = elems;
    args[i].N = N;
    args[i].k = k;
    args[i].i = i;
    pthread_mutex_unlock(&m);
    pthread_create(threads + i, NULL, thread_worker, (void *)(args + i));
  }

  for (int i = 0; i < k; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&m);

  for (int i = 0; i < k; i++) {
    printf("%.10g ", elems[i]);
  }

  printf("\n");

  free(threads);
  free(elems);

  return 0;

}