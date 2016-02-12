/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rdthreadpool.h"

using namespace std;

RDThreadPool RDThreadPool::pool;

RDThreadPool::~RDThreadPool()
{
    m_bstop = true;
    for(size_t i = 0; i < threads.size(); i++)
    {
        m_Tasks.push(shared_ptr<ThreadBaseTask>());
    }
}

RDThreadPool::RDThreadPool()
    :m_bstop(false)
     ,joiner(threads)
{
    const size_t thread_count = thread::hardware_concurrency();
    try{
        for(size_t i =0; i < thread_count; i++)
            threads.push_back(thread(&RDThreadPool::workerThread,this));
    }
    catch(...){
        m_bstop = true;
        throw;
    }
}

void RDThreadPool::workerThread()
{
    while(!m_bstop)
    {
        shared_ptr<ThreadBaseTask> task;
        m_Tasks.wait_and_pop(task);
        if(task)
            task->call();
        else
        {
            std::this_thread::yield();
        }
    }
}

joinThreads::joinThreads(vector<thread> &threads_)
        :threads(threads_)
{
}

joinThreads::~joinThreads()
{
    for(size_t i = 0; i < threads.size(); i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }

}
