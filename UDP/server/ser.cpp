#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main()
{
  
  // 创建套接字
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd < 0)
  {
    perror("socket");
    return -1;
  }
  
  // 绑定端口号
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(19999);
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");

  int ret = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
  if(ret < 0)
  {
     perror("bind");
     return -1;
  }



  while(1)
  {
    
    // 接收
    char buf[1024] = {0};
    struct sockaddr_in peer;
    socklen_t peer_len = sizeof(peer);
    ssize_t recv = recvfrom(sockfd, buf, sizeof(buf)-1, 0,(struct sockaddr*) &peer, &peer_len) ;
    if(recv < 0)
    {
      perror("recvfrom");
      return -1;
    }

    printf("cli : %s\n", buf);
    memset(buf, '\0', sizeof(buf));
    strncpy(buf, "woshi sserver", sizeof(buf));

    // 发送数据
    
    ssize_t send = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&peer, peer_len);

    if(send < 0)
    {
      perror("sendto");
      return -1;
    }
    // 打印应答
    }
  close(sockfd);
  return 0;
}
