#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H_ 

#include <iostream>
#include <string>
#include <string.h>
#include <unordered_map>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Dict{
  private:
    std::unordered_map<std::string, std::string>dict;
  public:
    Dict()
    {
      dict.insert({
          {"apple","苹果"},
          {"banana","香蕉"},
          {"old","老的"},
          {"phone","电话"},
          });
    }

};

class Server{
  private:
    std::string ip;
    short port;
    int listen_sock;
    static Dict d;
  public:
    Server(std::string _ip, short _port):ip(_ip), port(_port), listen_sock(-1)
  {}
    void InitServer()
        {
          listen_sock = socket(AF_INET, SOCK_STREAM, 0);
          if(listen_sock < 0){
           std::cerr << "socket error!" << std::endl;
           exit(2);
          }
           struct sockaddr_in local;
           bzero(&local, sizeof(local));
           local.sin_family = AF_INET;
           local.sin_port = htons(port);
           local.sin_addr.s_addr = inet_addr(ip.c_str());
           if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local))<0){
             std::cerr << "bind error!" << std::endl;
             exit(3);
           }
            if(listen(listen_sock, 5)<0){
             std::cerr << "listen error" << std::endl;
             exit(4);
            }
        }        
};


#endif  
