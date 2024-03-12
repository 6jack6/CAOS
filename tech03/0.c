#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int is_digit(char s);
char **divide_str(char* str);

int main(int argc, char* argv[]) {
  if (argc != 4) {
    return 1;
  }

  int in_fd = open(argv[1], O_RDONLY);
  int digit_fd = open(argv[2], O_WRONLY | O_CREAT, 0640);
  int other_fd = open(argv[3], O_WRONLY | O_CREAT, 0640);
  if (in_fd < 0 || digit_fd < 0 || other_fd < 0) {
    close(in_fd);
    close(digit_fd);
    close(other_fd);
    return 1;
  }

  int num_read;
  char buffer[1000];
  num_read = read(in_fd, buffer, sizeof(buffer) - 1);
  while (num_read > 0) {
    char **division = divide_str(buffer);
    if (write(other_fd, division[0], sizeof(char) * strlen(division[0])) <= 0) {
      close(in_fd);
      close(digit_fd);
      close(other_fd);
      return 1;
    }
    if (write(digit_fd, division[1], sizeof(char) * strlen(division[1])) <= 0) {
      close(in_fd);
      close(digit_fd);
      close(other_fd);
      return 1;
    }
    free(division[0]);
    free(division[1]);
    free(division);
    num_read = read(in_fd, buffer, sizeof(buffer) - 1);
  }
  if (num_read < 0) {
    close(in_fd);
    close(digit_fd);
    close(other_fd);
    return 1;
  }
}

int is_digit(char s) {
  return ('0' <= s && s <= '9');
}

char **divide_str(char* str) {
  int size = strlen(str);
  char **division = malloc(sizeof(char*) * 2);
  for (int i = 0; i < 2; ++i) {
    division[i] = malloc(sizeof(char) * (size + 1));
  }
  int ind1 = 0, ind2 = 0;
  while (*str != '\0') {
    if (is_digit(*str) == 0) {
      division[0][ind1++] = *str;
    } else {
      division[1][ind2++] = *str;
    }
    ++str;
  }
  division[0][ind1] = '\0';
  division[1][ind2] = '\0';
  return division;
}