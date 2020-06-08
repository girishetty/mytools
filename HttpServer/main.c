
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int serverProcess() {
  size_t kMaxClients = 10;
  size_t count = 0;
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFd == -1) {
    printf("Failed to Create the server Socket: error: %d\n", errno);
    return -1;
  }

  int enable = 1;
  setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
  struct sockaddr_in serverIp;
  serverIp.sin_family = AF_INET;
  serverIp.sin_port = htons(5000);
  inet_aton("127.0.0.1", (struct in_addr*)&serverIp.sin_addr.s_addr);

  if (-1 == bind(sockFd, (const struct sockaddr*)&serverIp, sizeof(serverIp))) {
    printf("Failed to Bind the server address: error: %d\n", errno);
    close(sockFd);
    return -1;
  }

  if (-1 == listen(sockFd, 10)) {
    printf("Failed to Listen on the server Socket: error: %d\n", errno);
    close(sockFd);
    return -1;
  }

  int requestFd = -1;
  int a = 0;
  int b = 0;
  int sum = 0;
  int readCount = 0;
  char clientRequest[50] = {};
  char response[50] = {};

  while (1) {
    // start accepting connection requests from the clients
    requestFd = accept(sockFd, NULL, 0);
    if (requestFd == -1) {
      // Notify/Log about failure to accept a request, but continue accetping more clients
      printf("Failed to Listen on the server Socket: error: %d\n", errno);
      close(requestFd);
      continue;
    }
    // start reading requests from this client.
    readCount = read(requestFd, clientRequest, 50);
    if (readCount > 0 && (strncmp(clientRequest, "GET /sum?a=", 11) == 0)) {
      // Now parse it to get the values of a and b.
      readCount = sscanf(clientRequest + 11, "%d&b=%d", &a, &b);
      if (2 != readCount) {
        // break on invalid request.
        printf("Failed to Parse the client Request\n");
        close(requestFd);
        continue;
      }
      sum = a + b;
      sprintf(response, "HTTP/1.0 200 OK\n\r\n\r%d\n\r", sum);
      if (-1 == write(requestFd, response, strlen(response))) {
      }
    } else if (readCount == -1) {
      printf("Failed to Read from the client: error: \n", errno);
    } else {
      printf("Invalid Request from the client\n");
    }
    close(requestFd);
    if (count++ == kMaxClients) {
      printf("Ending the Server as we have served %zu clients\n", kMaxClients);
      break;
    }
  }

  close(sockFd);
  return 0;
}

int main() {
  serverProcess();
  return 0;
}
