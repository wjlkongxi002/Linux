#include "ChatServer.hpp"

int main()
{
  ChatServer* cs = new ChatServer();
  if(!cs)
  {
    printf("Init ChatServer Failed\n");
    return -1;
  }

  cs->InitSvr();
  cs->start();
  return 0;
}
