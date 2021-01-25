#pragma once 
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TCP_PORT 17878


class ChatServer
{
  public:
    ChatServer()
    {
      // 登录注册
      tcp_sock_ = -1;
      tcp_port_ = TCP_PORT;
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
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, LoginRegisterStart, NULL);
    
      }
    }

  private:
    static void* LoginRegisterStart()
    {
      /*
       *1.注册

       *2.登录
       * */
    }


  private:
    int tcp_sock_;
    uint16_t tcp_port_;
    int udp_sock_;
};
