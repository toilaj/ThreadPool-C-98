#include "ThreadPool.h"
#include "iostream"
ThreadPool::ThreadPool(int num)
{
    m_ThreadsNum = num;
    m_IsRunning = true;
    pthread_mutex_init(&m_Mutex, NULL);
    pthread_cond_init(&m_Condition, NULL);
    CreateThreads();
}


ThreadPool::~ThreadPool()
{
    ClearThreads();
    ClearQueue();
}


ThreadPool* ThreadPool::CreateThreadPool(int num)
{
    static ThreadPool* pInstance = new ThreadPool(num);
    return pInstance;
}


void ThreadPool::CreateThreads()
{
    m_pThreadSet = (pthread_t*)malloc(sizeof(pthread_t) * m_ThreadsNum);
    for(int i=0; i < m_ThreadsNum; i++) {
        pthread_create(&m_pThreadSet[i], NULL, ThreadFunc, this);
    }
}


void* ThreadPool::ThreadFunc(void* arg)
{
    ThreadPool* p = (ThreadPool*)arg;
    while(p->m_IsRunning) {
        Task* task = p->GetTask();
        if(!task) {
            break;
        }
        task->run();
        delete task;
    }
    return NULL;
}


void ThreadPool::ClearQueue()
{
    while(!m_TaskQueue.empty()) {
        delete (m_TaskQueue.front());
        m_TaskQueue.pop();
    }
}


void ThreadPool::ClearThreads()
{
    if(!m_IsRunning) {
        return;
    }
    m_IsRunning = false;
    pthread_cond_broadcast(&m_Condition);
    for (int i = 0; i < m_ThreadsNum; i++) {
        pthread_join(m_pThreadSet[i], NULL);
    }
    free(m_pThreadSet);
    m_pThreadSet = NULL;
    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_Condition);
}


int ThreadPool::GetQueueSize()
{
    pthread_mutex_lock(&m_Mutex);
    size_t size = m_TaskQueue.size();
    pthread_mutex_unlock(&m_Mutex);
    return size;
}


int ThreadPool::GetThreadlNum()
{
    return m_ThreadsNum;
}


void ThreadPool::AddTask(Task* pTask)
{
    pthread_mutex_lock(&m_Mutex);
    m_TaskQueue.push(pTask);
    std::cout << "push a task, current element are: " << m_TaskQueue.size() << std::endl;
    pthread_mutex_unlock(&m_Mutex);
    pthread_cond_signal(&m_Condition);
}


Task* ThreadPool::GetTask()
{
    Task* pTask = NULL;
    while(!pTask) {
        pthread_mutex_lock(&m_Mutex);
        while(m_TaskQueue.empty() && m_IsRunning) {
            pthread_cond_wait(&m_Condition, &m_Mutex);
        }

        if(!m_IsRunning) {
            pthread_mutex_unlock(&m_Mutex);
            break;
        }
        else if(m_TaskQueue.empty()) {
            pthread_mutex_unlock(&m_Mutex);
            continue;
        }
        pTask = m_TaskQueue.front();
        m_TaskQueue.pop();
        pthread_mutex_unlock(&m_Mutex);
    }
    return pTask;
}
