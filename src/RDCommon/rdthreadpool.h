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

#ifndef RDTHREADPOOL_H
#define RDTHREADPOOL_H
#include "rdthreadsafepriorityqueue.h"
#include <vector>
#include <atomic>
#include <thread>
#include <future>

class ThreadBaseTask
{
    char m_nPriority;
public:
    ThreadBaseTask(char priority){
        m_nPriority = priority;
    }

    virtual ~ThreadBaseTask() = default;
    virtual void call()=0;
    virtual void wait()=0;

    bool operator < (const ThreadBaseTask& other)
    {
        return m_nPriority < other.m_nPriority;
    }
};
template<typename Result>
class ThreadTask:public ThreadBaseTask
{
public:
    template<typename F> ThreadTask(F&& f,char priority = 2)
        :ThreadBaseTask(priority)
         ,m_task(new std::packaged_task<Result()>(std::move(f)))
         //,m_task(new packaged_task<Result()>(f))
         ,m_Res(m_task->get_future())
         ,m_bRun(false)
    {
    }

    ThreadTask(ThreadTask&& other)
        :m_task(std::move(other.m_task))
         ,m_Res(std::move(other.m_Res))
         ,m_bRun(std::move(other.m_bRun))
    {}

    ThreadTask& operator= (ThreadTask&& other)
    {
        m_task = std::move(other.m_task);
        m_Res = std::move(other.m_Res);
        m_bRun = std::move(other.m_bRun);
        return *this;
    }

    void call(){
        bool bRun = std::atomic_exchange(&m_bRun,true);
        if(bRun)
            return;
        (*m_task)();
    }

    void wait(){
        getValue();
    }
    Result getValue()
    {
        call();
        return m_Res.get();
    }

    ThreadTask(ThreadTask&) = delete;
    ThreadTask(const ThreadTask&) = delete;
    ThreadTask& operator=(const ThreadTask&) = delete;
protected:
    std::unique_ptr<std::packaged_task<Result()>> m_task;
    std::future<Result>						m_Res;
    std::atomic_bool m_bRun;
};

class joinThreads
{
    std::vector<std::thread>& threads;
public:
    explicit joinThreads(std::vector<std::thread>& threads_);
    ~joinThreads();
};

class RDThreadPool
{
public:
    RDThreadPool(const RDThreadPool&)=delete;
    template<typename F>
    static std::shared_ptr<ThreadTask<typename std::result_of<F()>::type>> AddTask(F f)
    {
        typedef typename std::result_of<F()>::type result_type;
        //shared_ptr<ThreadTask<result_type>> ret =
        auto ret = std::make_shared<ThreadTask<result_type>>(f);
        pool.m_Tasks.push(ret);
        return ret;
    }
    ~RDThreadPool();
protected:
    RDThreadPool();
    void workerThread();
protected:
    static RDThreadPool pool;
protected:
    RDPriorityQueue<std::shared_ptr<ThreadBaseTask>> m_Tasks;
    std::vector<std::thread> threads;
    std::atomic_bool m_bstop;
    joinThreads joiner;
};

#endif // RDTHREADPOOL_H
