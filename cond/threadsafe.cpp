#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define Thread_Count 2
// 0 表示没有面
// 1 表示有面

int g_bowl = 0;

pthread_mutex_t g_tex;
pthread_cond_t cond;  // 消费者    PCB等待队列
pthread_cond_t pro_cond; // 生产者
void* EatNoodles(void* arg)
{
  pthread_detach(pthread_self());
  (void)arg;// 强转，arg是一个无符号的指针
  while(1)
  {
    pthread_mutex_lock(&g_tex);
    while(g_bowl <= 0)
    {
      // 等待
      pthread_cond_wait(&cond, &g_tex);
    }
    g_bowl--;
    printf("Eat---g_bowl:%d\n", g_bowl);
    pthread_mutex_unlock(&g_tex);
    // 通知做面的人
    pthread_cond_signal(&pro_cond);
  }
  return NULL;
}

void* MakeNoodles(void* arg)
{
  pthread_detach(pthread_self());
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&g_tex);
    while(g_bowl > 0)
    {
      // 等待
      pthread_cond_wait(&pro_cond, &g_tex);
    }
    g_bowl++;
    printf("Make---g_bowl:%d\n", g_bowl);
    pthread_mutex_unlock(&g_tex);
    // 通知吃面的人
    pthread_cond_signal(&cond);
  }
  return NULL;
}



int main()
{
  pthread_mutex_init(&g_tex, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_cond_init(&pro_cond, NULL);
  pthread_t tid;

  for(int i=0;i < Thread_Count; i++)
  {
    int ret = pthread_create(&tid, NULL, EatNoodles, NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }

    ret = pthread_create(&tid, NULL, MakeNoodles, NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  while(1)
  {

    sleep(1);
  }
  pthread_mutex_destroy(&g_tex);
  pthread_cond_destroy(&cond);
  pthread_cond_destroy(&pro_cond);
  return 0;
}
