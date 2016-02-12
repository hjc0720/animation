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

#ifndef RDTHREADSAFEPRIORITYQUEUE
#define RDTHREADSAFEPRIORITYQUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class RDPriorityQueue
{
public:
    RDPriorityQueue(){}
    RDPriorityQueue(const RDPriorityQueue& other){
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    RDPriorityQueue& operator=(const RDPriorityQueue&) = delete;
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.top();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value = std::move(data_queue.top());
        data_queue.pop();
        return true;
    }
protected:
    mutable std::mutex mut;
    std::priority_queue<T> data_queue;
    std::condition_variable data_cond;
};

#endif // RDTHREADSAFEPRIORITYQUEUE

