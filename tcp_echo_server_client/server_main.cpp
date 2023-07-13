#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

bool is_int(char *c) {
  while (*c != '\0') {
    if (*c < '0' || *c > '9')
      return false;
    c++;
  }
  return true;
}

int main(int argc, char **argv) {
  if (argc != 2 || !is_int(argv[1])) {
    std::cerr << "[ERROR] Port is not provided via command line parameters! \n";
    return -1;
  }
  int sock_listener = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_listener < 0) {
    std::cerr << "[ERROR] Socket cannot be created!\n";
    return -2;
  }

  std::cout << "[INFO] Socket has been created.\n";
  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(std::atoi(argv[1]));
  inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

  char buf[INET_ADDRSTRLEN];

  if (bind(sock_listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "[ERROR] Created socket cannot be binded to ("
              << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
              << ":" << ntohs(server_addr.sin_port) << ")\n";
    return -3;
  }
  std::cout << "[INFO] Sock is binded to ("
            << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
            << ":" << ntohs(server_addr.sin_port) << ")\n";

  if (listen(sock_listener, SOMAXCONN) < 0) {
    std::cerr << "[ERROR] Socket cannot be switched to listen mode!\n";
    return -4;
  }
  std::cout << "[INFO] Socket is listening now.\n";
  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int sock_client;
  if ((sock_client = accept(sock_listener, (sockaddr *)&client_addr,
                            &client_addr_size)) < 0) {
    std::cerr << "[ERROR] Connection cannot be accepted for a reason.\n";
    return -5;
  }
  std::cout << "[INFO] A connection is accepted now\n";

  close(sock_listener);
  std::cout << "[INFO] Main listener socket is closed.\n";

  char host[NI_MAXHOST];
  char svc[NI_MAXSERV];

  if (getnameinfo((sockaddr *)&client_addr, client_addr_size, host, NI_MAXHOST,
                  svc, NI_MAXSERV, 0) != 0) {
    std::cout << "[INFO] Clienet : ("
              << inet_ntop(AF_INET, &client_addr.sin_addr, buf, INET_ADDRSTRLEN)
              << ":" << ntohs(client_addr.sin_port) << ")\n";
  } else {
    std::cout << "[INFO] Client: (host : " << host << ", service: " << svc
              << ")\n";
  }
  char msg_buf[4096];
  int bytes;

  while (true) {
    bytes = recv(sock_client, &msg_buf, 4096, 0);
    if (bytes == 0) {
      std::cout << "[INFO] Client is disconnected.\n";
      break;
    } else if (bytes < 0) {
      std::cerr << "[ERROR] Something went wrong receiving data!\n";
      break;
    }
    else 
    {
      std::cout << "client > " << std::string(msg_buf, 0, bytes) << "\n";
      if (send(sock_client, &msg_buf, bytes, 0) < 0)
      {
        std::cerr << "[ERROR] Message cannot be send, exiting ...\n";
        break;
      }
    }
  }
  close(sock_client);
  std::cout << "[INFO] Client socket is closed.\n";
  return 0;
}
