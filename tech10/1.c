#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>


sig_atomic_t end = 0;
sig_atomic_t is_accepting = 0;


void handler(int signum) {
  end = 1;
  if (is_accepting) {
    exit(0);
  }
}
int main(int argc, char** argv) {
  if (argc != 3) {
    perror("incorrect args\n");
    exit(1);
  }

  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(strtol(argv[1], NULL, 10)),
    .sin_addr.s_addr = inet_addr("127.0.0.1"),
  };

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 1) {
    perror("socket error\n");
    exit(1);
  }

  sigset_t ignored;
  sigfillset(&ignored);
  sigdelset(&ignored, SIGINT);
  sigdelset(&ignored, SIGTERM);
  sigprocmask(SIG_BLOCK, &ignored, NULL);

  struct sigaction sigint  = {
    .sa_handler = handler,
    .sa_flags   = SA_RESTART
  };

  sigaction(SIGINT,  &sigint, NULL);
  sigaction(SIGTERM, &sigint, NULL);

  if (bind(socket_fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind error\n");
    exit(1);
  }
  if (listen(socket_fd, 128) == -1) {
    perror("listen error\n");
    exit(1);
  }

  const char query[] = "GET %s HTTP/1.1";
  const char str_ok[] = "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n";
  const char str_not_found[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
  const char str_forbidden[] = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";


  while(!end) {
    is_accepting = 1;
    int connection_socket = accept(socket_fd, NULL, NULL);
    is_accepting = 0;

    char filename[1024];
    int size = sprintf(filename, "%s/", argv[2]);

    FILE* file = fdopen(dup(connection_socket), "r");
    fscanf(file, query, filename + size);
    if (access(filename, F_OK) != 0) {
      dprintf(connection_socket, str_not_found);

    }
    else if (access(filename, R_OK) != 0) {
      dprintf(connection_socket, str_forbidden);
    }
    else {
      struct stat stats;
      lstat(filename, &stats);
      dprintf(connection_socket, str_ok, stats.st_size);


      int fd = open(filename, O_RDONLY);
      char buffer[4096];
      while (read(fd, buffer, 4096) > 0) {
        dprintf(connection_socket, "%s", buffer);
      }
      close(fd);
    }

    fclose(file);
    shutdown(connection_socket, SHUT_RDWR);
    close(connection_socket);
  }

  close(socket_fd);
}