#include "udp_server.hpp"

void Usage(std::string _port)
{
  std::cout<<_port<<"ip port"<<std::endl;
}

//./udp_server ip port 
int main(int argc,char *argv[])//命令行参数，argc 参数个数，argv[]存命令行参数首地址
{
  if(argc != 3)
  {
    Usage(argv[0]);
    exit(1);
  }
  Server *sp = new Server(argv[1], atoi(argv[2]));
  sp->InitServer();
  sp->Run();
  return 0;

}
