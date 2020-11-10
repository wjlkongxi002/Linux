#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define THREAD_COUNT 2

int g_tickets = 100;

pthread_mutex_t tex;
pthread_mutex_t tex2;

struct ThreadData
{
  int num_;
  ThreadData()
  {
    num_ = -1;
  }
};


void* ThreadStart(void* arg)
{
  pthread_detach(pthread_self());
  pthread_mutex_lock(&tex);

  sleep(5);//sleep原因是怕太快了

  pthread_mutex_lock(&tex2);

}

void* ThreadStart2(void* arg)
{
  pthread_detach(pthread_self());
  pthread_mutex_lock (&tex2);

  sleep(5);

  pthread_mutex_lock(&tex);

}
int main()
{
  pthread_mutex_init(&tex, NULL);
  pthread_mutex_init(&tex2, NULL);
  pthread_t tid;// pthread_t 是一个类型
  int ret = pthread_create(&tid, NULL, ThreadStart, NULL);
  if(ret < 0)
  {
    perror("pthread_create");
    return -1;
  }

  ret = pthread_create(&tid, NULL, ThreadStart2, NULL);
  if(ret < 0)
  {
    perror("pthread_create");
    return -1;
  }

  while(1)
  {
    printf("i am main thread\n");
    sleep(1);
  }
  pthread_mutex_destroy(&tex);
  pthread_mutex_destroy(&tex2);
  return 0;
}
