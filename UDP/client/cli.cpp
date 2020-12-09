#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>

int main()
{
  
  // 创建套接字
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd < 0)
  {
    perror("socket");
    return -1;
  }
  
  while(1)
  {  
    char buf[1024] = {0};
    memset(buf, '\0', sizeof(buf));
    std::cin >> buf;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19999);
    addr.sin_addr.s_addr = inet_addr("192.168.236.136");

    ssize_t send = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, sizeof(addr));

    if(send < 0)
    {
      perror("sendto");
      return -1;
    }


    memset(buf, '\n', sizeof(buf));
    ssize_t recv = recvfrom(sockfd, buf, sizeof(buf)-1, 0, NULL, NULL);
    if(recv < 0)
    {
      perror("recvfrom");
      return -1;
    }
    printf("ser : %s\n", buf);

    }
  close(sockfd);
  return 0;
}
