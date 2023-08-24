#include "Webserv.hpp"

Webserv::Webserv(std::string config)
  : _kq(0)
{
  std::cout << "--- parseConfig ---\n";
  _servers.parseConfig(config);

}

void Webserv::initServers()
{
  if (_kq != 0)
  {
    std::cout << "kqueue already initialized!\n";
    return ;
  }

  _kq = kqueue();
  if (_kq == -1)
  {
    std::cout << "kqueue() error" <<std::endl;
    exit(1);
  }

  std::map<int, Server>::iterator it = _servers.server.begin();
  for (std::map<int, Server>::iterator it = _servers.server.begin();
        it != _servers.server.end(); it++)
  {
    int serverSocket = it->second.openPort();
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);
  }
}

Webserv::~Webserv(){}
