#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

void add(char* location, unsigned int number, unsigned int W) {
  char a = location[W];
  snprintf(location, 4096, "%*d", W, number);
  location[W] = a;
}

off_t file_size(int fd) {
  struct stat st;
  fstat(fd, &st);
  return st.st_size;
}

int main(int argc, char* argv[]) {
  unsigned int W = strtol(argv[3], NULL, 10);
  unsigned int N = strtol(argv[2], NULL, 10);

  int fd = open(argv[1],  O_CREAT | O_RDWR, 0640);
  if (fd == -1) {
    close(fd);
    perror("file opening error\n");
    exit(1);
  }

  if (ftruncate(fd, N * N * W + N) == -1) {
    close(fd);
    perror("ftruncate error\n");
  }

  if (file_size(fd) == 0) {
    perror("file size error\n");
    exit(3);
  }

  char *file_data = mmap(NULL, N * N * W + N, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (file_data == MAP_FAILED) {
    perror("file mapping error\n");
    exit(4);
  }

  int Ibeg = 0, Ifin = 0, Jbeg = 0, Jfin = 0;
  int k = 1;
  int i = 0;
  int j = 0;
  while (k <= N * N) {
    add(file_data + ((W * N + 1) * i + j * (W)), k++, W);
    if (i == Ibeg && j < N - Jfin - 1)
      ++j;
    else if (j == N - Jfin - 1 && i < N - Ifin - 1)
      ++i;
    else if (i == N - Ifin - 1 && j > Jbeg)
      --j;
    else
      --i;
    if ((i == Ibeg + 1) && (j == Jbeg) && (Jbeg != N - Jfin - 1)) {
      ++Ibeg;
      ++Ifin;
      ++Jbeg;
      ++Jfin;
    }
  }
  for (i = 0; i < N; i++) {
    file_data[W * (N * i + N) + i] = '\n';
  }

  if (msync(file_data, N * N * W + N, MS_SYNC) == -1) {
    close(fd);
    perror("map synchronisation error\n");
    exit(1);
  }

  if (munmap(file_data, N * N * W + N) == -1) {
    close(fd);
    perror("file unmapping error\n");
    exit(1);
  }
  close(fd);
  return 0;
}