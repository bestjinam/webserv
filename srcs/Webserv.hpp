#pragma once 
#include "Servers.hpp"
#include <string>
#include <sys/event.h>
#include <fcntl.h>
#include <vector>

class Webserv
{
  private:
    Servers _servers;
    int     _kq;
    std::vector<struct kevent> _changelist;

    Webserv(Webserv const & obj);
    Webserv const & operator=(Webserv const &obj);
  public:
    Webserv(std::string s);
    ~Webserv();

    //Servers 
    void initServers(void);
    void exitWebServ(std::string msg);
};
