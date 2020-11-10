#include <stdio.h>
#include <pthread.h>
#include <unistd.h>



#define THREAD_COUNT 4

int g_tickets = 100;

pthread_mutex_t tex;

struct ThreaddData
{
  int num_;
  ThreaddData()
  {
    num_ = -1;
  }
};



void* ThreadStart(void* arg)
{
  pthread_detach(pthread_self());
  ThreaddData* td = (ThreaddData*)arg;
  while(1)
  {
    pthread_mutex_lock(&tex);
    if(g_tickets > 0)
    {
      printf("i am %p-%d, i have ticket is %d\n", pthread_self(), td->num_, g_tickets);
      g_tickets--;
    }
    else 
    {
      pthread_mutex_unlock(&tex);
      break;
    }
    pthread_mutex_unlock(&tex);
  }
  delete td;
}




int main()
{
  pthread_mutex_init(&tex, NULL);
  pthread_t tid[THREAD_COUNT];
  for(int i = 0; i < THREAD_COUNT; i++)
  {
    struct ThreaddData* td = new ThreaddData;
    td->num_ = i;
    int ret = pthread_create(&tid[i], NULL, ThreadStart, (void*)td);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }





  while(1)
  {
    //printf("i am main thread\n");
    sleep(1);
  }
  pthread_mutex_destroy(&tex);
  return 0;
}

