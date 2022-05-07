#include <queue>
#include <unistd.h>
#include <pthread.h>

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#define DEFAULT_MAX_THREADS 10
using namespace std;
class Task
{
public:
    Task(void* a = NULL) { m_arg = a; }
    virtual ~Task(){};
    void SetArg(void* a) { m_arg = a; }
    virtual bool run() = 0;
protected:
    void* m_arg;
};

class ThreadPool
{
public:
    void AddTask(Task* pTask);
    static ThreadPool* CreateThreadPool(int num=10);
    ~ThreadPool();
    int GetQueueSize();
    int GetThreadlNum();
private:
    /*singleton, construction function is private*/
    ThreadPool(int num = DEFAULT_MAX_THREADS);
    void CreateThreads();
    void ClearQueue();
    void ClearThreads();
    Task* GetTask();
    static void* ThreadFunc(void* arg);
    std::queue<Task*> m_TaskQueue;
    pthread_t* m_pThreadSet;
    pthread_mutex_t m_Mutex;
    pthread_cond_t m_Condition;
    bool m_IsRunning;
    int m_ThreadsNum;
};


#endif //THREAD_POOL_THREAD_POOL_H