#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main() {
  char request_str[1000];
  char host_str[1000];
  fgets(request_str, sizeof(request_str), stdin);
  fgets(host_str, sizeof(host_str), stdin);
  request_str[strlen(request_str) - 1] = '\0';
  host_str[strlen(host_str) - 1] = '\0';
  char* http_host;
  char* query_string;
  char* request_method;
  char* script_name;
  char* protocol;
  http_host = strchr(host_str, ' ') + 1;
  request_method = request_str;
  script_name = strchr(request_str, ' ');
  *script_name = '\0';
  script_name += 2;
  protocol = strchr(script_name, ' ');
  query_string = strchr(script_name, '?');
  if (query_string == NULL) {
    query_string = "\0";
  } else {
    *query_string = '\0';
    query_string++;
  }
  *protocol = '\0';
  protocol++;
  //printf("%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n", request_method, script_name, query_string, http_host, protocol);
  setenv("REQUEST_METHOD", request_method, 1);
  setenv("SCRIPT_NAME", script_name, 1);
  setenv("QUERY_STRING", query_string, 1);
  setenv("HTTP_HOST", http_host, 1);
  if (access(script_name, F_OK) == -1) {
    printf("%s 404 ERROR\n\n", protocol);
    exit(1);
  }
  if (access(script_name, X_OK) == -1) {
    printf("%s 403 ERROR\n\n", protocol);
    exit(1);
  }
  char exec_str[1000] = "./";
  if (strchr(script_name, '/') == NULL) {
    script_name = strcat(exec_str, script_name);
  }
  printf("%s 200 OK\n", protocol);
  fflush(stdout);
  execlp(script_name, script_name, NULL);
  return 0;
}