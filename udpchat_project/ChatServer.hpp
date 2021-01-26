#pragma once 
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "tools.hpp"
#include "ConnnectInfo.hpp"
#include "UserManager.hpp"

#define TCP_PORT 17878


class TcpConnect
{
  public:
    TcpConnect()
    {
      new_sock_ = -1;

    }
    ~TcpConnect()
    {

    }
    void SetSockFd(int fd)
    {
      new_sock_ = fd;
    }

    void SetServer(void* server)
    {
      server_ = server;
    }

    
    int GetSockFd()
    {
      return new_sock_;
    }

    void* GetServer()
    {
      return server_;
    }

  private:
    int new_sock_;

    // 保存ChatServer这个类的this指针，确保在tcp的线程入口函数当中可以获取到用户管理模块的实例化指针
    void* server_;
};
class ChatServer
{
  public:
    ChatServer()
    {
      // 登录注册
      tcp_sock_ = -1;
      tcp_port_ = TCP_PORT;
      user_manager_ = NULL;
    }

    ~ChatServer()
    {

    }

    /*
     * 初始化变量（服务）接口，被调用者调用的接口
     * 用户管理模块的实例化对象，消息池的实例化对象
     */
    int InitSvr(uint16_t tcp_port = TCP_PORT)
    {
      //1.创建tcp-socket，并且绑定地址信息，监听
      // 注册+登录模块
      tcp_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(tcp_sock_ < 0)
      {
        return -1;
      }
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(tcp_port);
      addr.sin_addr.s_addr = inet_addr("0.0.0.0");
      int ret = bind(tcp_sock_, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0)
      {
        return -2;
      }

      // 监听
      ret = listen(tcp_sock_, 5);
      if(ret < 0)
      {
        return -3;
      }

      std::string msg = "listen port is 17878";
      Log(INFO, __FILE__, __LINE__, msg) << std::endl;

      // 创建用户管理模块指针
      
    user_manager_ = new UserManager();
    if(!user_manager_)
    {
      Log(INFO, __FILE__, __LINE__, msg) << std::endl;
      return -1;
    }

      //暂时还没有考虑的是udp通信，以及登录注册模块，消息池的初始化
      
      return 0;
    }


    /*
     * 启动线程
     */
    int start()
    {
      /*
       * udp应该有两类线程
       *  1类：生产线程， 负责接收udp数据，将udp数据放在消息池中
       *  2类：消费线程，负责从消息池当中获取消息，发送到在线用户的客户端
       *  tcp
       *  针对每一个注册登录请求（tcp连接），针对每一个tcp连接，都创建一个线程，
       *  专门为该客户端处理注册和登录请求。注册和登录完成之后，服务器，关闭连接，销毁线程
       *
       *  tcp是否创建线程，取决于accept函数是否调用成功（阻塞）
       * 
       * */

      //TODO
      //udp 线程的创建
      
      
      
      struct sockaddr_in peer_addr;
      socklen_t peer_addrlen = sizeof(peer_addr);
      while(1)
      {
        int new_sock = accept(tcp_sock_, (struct sockaddr*)&peer_addr, &peer_addrlen);

          if(new_sock < 0)
          {
            continue;
          }
        //正常接收到了
        //创建线程，为客户端的注册和登录请求负责
        
          TcpConnect* tc = new TcpConnect();
          tc->SetSockFd(new_sock);
          tc->SetServer((void*)this);
        

          pthread_t tid;
          int ret = pthread_create(&tid, NULL, LoginRegisterStart, (void*)tc);
          if(ret < 0)
          {
            close(new_sock);
            delete tc;
            continue;
          }
    
      }
    }

  private:
    static void* LoginRegisterStart(void* arg)
    {
      /*
       * 1.分离自己，当线程退出之后，线程占用的资源就被操作系统回收了
       * 2.接收1个字节的数据，进而判断数据请求的类型，根据不同的请求类型，调用不同的函数进行处理
       
       *注册
       *登录
       * */

      pthread_detach(pthread_self());
      TcpConnect* tc = (TcpConnect*)arg;
      ChatServer* cs = (ChatServer*)tc->GetServer();

      char ques_type = -1;
      // tcp原生的接收函数 接收新连接
      ssize_t recv_size = recv(tc->GetSockFd(), &ques_type, 1, 0);
      if(recv_size < 0)
      {
        close(tc->GetSockFd());
        return NULL;
      }
      else if(recv_size == 0)
      {
        // recv_size等于0的时候，表示说对端将连接关闭了，我们的策略是
        // 关闭自己对该连接保存的套接字描述符
        close(tc->GetSockFd());
        return NULL;
      }

      // 接收回来一个字节的数据
      
      switch(ques_type)
      {
        case REGISTER_RESQ:
          {
            // 处理注册请求，DealRegister
            cs->DealRegister(tc->GetSockFd(), cs);
            break;
          }
        case LOGIN_RESQ:
          {
            // 处理登录请求
           break;
          }
      }

    }

    // 不管注册成功 还是失败，都会给客户端返回一个应答
    int DealRegister(int new_sock, ChatServer* cs)
    {
      // 继续从tcp连接当中接收注册数据，策略是：直接使用RegisterInfo
      struct RegisterInfo ri;
      ssize_t recv_size = recv(new_sock, &ri, sizeof(ri), 0);
      if(recv_size < 0)
      {
        return -1;
      }
      else if(recv_size == 0)
      {
        close(new_sock);
        return -2;
      }
      
        // 正常接收到了
        // 需要将数据递交给用户管理模块，进行注册，并且将用户数据进行存留
        // 需要和用户管理模块进行交互了
        
        cs->user_manager_->DealRegister(ri.nick_name_, ri.school_, ri.passwd_);

    }

    int DealLogin()
    {
      // 继续从tcp连接当中接收登录数据，策略是直接使用LoginInfo
    }


  private:
    int tcp_sock_;
    uint16_t tcp_port_;
    int udp_sock_;

    UserManager* user_manager_;
};
