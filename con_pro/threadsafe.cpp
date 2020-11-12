//  生产者消费者模型

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>

#define CAPACITY 5 // 容量
#define Thread_Count 2 // 线程数

pthread_mutex_t g_mutex;

// 定义一个线程安全的队列

class SafeQueue
{
  public:
    SafeQueue()
    {
      capacity_ = CAPACITY;
      pthread_mutex_init(&mutex_, NULL);
      pthread_cond_init(&con_cond_, NULL);
      pthread_cond_init(&pro_cond_, NULL);
    }

    ~SafeQueue()
    {
      pthread_mutex_destroy(&mutex_);
      pthread_cond_destroy(&con_cond_);
      pthread_cond_destroy(&pro_cond_);

    }


    void Push(int data)
    {
      pthread_mutex_lock(&mutex_);
      while(queue_.size() >= capacity_)
      {
        pthread_cond_wait(&con_cond_, &mutex);
      }
      *data = queue_.front();
      queue_.pop();


    }

  private:
    std::queue<int> queue_;
    size_t capacity_; // 给定队列容量

    pthread_mutex_t mutex_; // 互斥锁
    pthread_cond_t con_cond_; // 消费者条件变量
    pthread_cond_t pro_cond_; // 生产者条件变量
};

void* con_start(void* arg)
{
  SafeQueue* sq = (SafeQueue*)arg;
  while(1)
  {
    int data;
    sq-->Pop(&data);
    printf("i am %p, i consume %d\n", pthread_self(), data);
  }
  return NULL;
}

int count = 0;
void* pro_start(void* arg)
{
  SafeQueue* sq = (SafeQueue*)arg;
  while(1)
  {
    pthread_mutex_lock(&g_mutex);
    sq->Push(count);
    printf("i am %p, i product %d\n", pthread_self(), count);
    count ++;
    pthread_mutex_unlock(&g_mutex);
  }
  return NULL;
}

int main()
{
  pthread_mutex_init(&g_mutex, NULL); 
  SafeQueue* sq = new SafeQueue();
  pthread_t con[Thread_Count], pro[Thread_Count];
  for(int i = 0; i < Thread_Count; i++)
  {
    int ret = pthread_create(&con[i], NULL, con_start, (void*)sq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }

    ret = pthread_create(&pro[i], NULL, pro_start, (void*)sq);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }

    for(int i = 0; i < Thread_Count;  i++)
    {
      pthread_join(con[i], NULL);
      pthread_join(pro[i], NULL);
    }


  delete sq;
  pthread_mutex_destroy(&g_mutex);
  return 0;
}
