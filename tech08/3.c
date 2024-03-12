#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
  pid_t *pids = calloc(argc - 1, sizeof(pid_t));
  int **fds_in = calloc(argc - 1, sizeof(int*));
  int **fds_out = calloc(argc - 1, sizeof(int*));
  for (int i = 0; i < argc - 1; ++i) {
    fds_in[i] = calloc(2, sizeof(int));
    fds_out[i] = calloc(2, sizeof(int));
    if (pipe(fds_out[i]) == -1) {
      perror("pipe error\n");
      exit(1);
    }
    if (pipe(fds_in[i]) == -1) {
      perror("pipe error\n");
      exit(1);
    }
  }
  for (int i = 0; i < argc - 1; ++i) {
    pids[i] = fork();
    if (pids[i] == -1) {
      perror("forking error\n");
      exit(1);
    }
    // sub process
    if (pids[i] == 0) {
      if (i < argc - 2) {
        if (dup2(fds_out[i][1], STDOUT_FILENO) == -1) {
          perror("duplication error\n");
          exit(1);
        }
//        close(fds_out[i][0]);
        close(fds_out[i][1]);
      }
      if (i > 0) {
        if (dup2(fds_in[i][0], STDIN_FILENO) == -1) {
          perror("duplication error\n");
          exit(1);
        }
        close(fds_in[i][0]);
        close(fds_in[i][1]);
      }
      execlp(argv[i + 1], argv[i + 1], NULL);
      perror("execution error\n");
      exit(1);
    }
    //main process
    close(fds_out[i][1]);
    close(fds_in[i][0]);
    close(fds_in[i][1]);
    if (i < argc - 2) {
      char buffer[4096];
      int tmp_size;
      while((tmp_size = read(fds_out[i][0], buffer, sizeof(buffer))) > 0) {
        write(fds_in[i + 1][1], buffer, tmp_size);
        break;
      }
    }
    close(fds_out[i][0]);
  }
  for (int i = 0; i < argc - 1; ++i) {
    if (waitpid(pids[i], NULL, 0) == -1) {
      perror("waiting for children to die error\n");
      exit(1);
    }
    free(fds_in[i]);
    free(fds_out[i]);
  }
  free(pids);
  free(fds_in);
  free(fds_out);
}