#include "ThreadPool.h"
#include <iostream>

class PrintTask: public Task{
public:
    PrintTask(void *arg) {
        m_arg = arg;
    }
    bool run() {
        for(int i = 0; i < 10; i++) {
            cout << "this is thread: " << *(int*)m_arg << " seq: " << i << endl;
            sleep(1);
        }
        return true;
    }
};

int main()
{
    int i, thread_id[10];
    ThreadPool *pool = ThreadPool::CreateThreadPool(5);
    for(i =0; i < 10; i++) {
        thread_id[i] = i;
        pool->AddTask(new PrintTask(&thread_id[i]));
    }
    while(pool->GetQueueSize() != 0) {
        sleep(1);
    }
    sleep(10);
    cout << "all task end!" << endl;
    return 0;
}