// =====================================================================================
// 
//       Filename:  RDThread.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  14/04/11 21:16:42
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDThread.h"
#include <QThread>
#include "mac_define.h"

class MyThread : public QThread
{
public:
    MyThread(RDThread* parent)
        :QThread(0)
    {
        m_pParent = parent;
    }
    void run()
    {
        m_pParent->RunThread();
    }
protected:
    RDThread* m_pParent;

};

RDThread::RDThread(RDThread_CallBack pCallBack,void* pCallBackParam)
{
    m_pCallBack = pCallBack;
    m_pCallBackParam = pCallBackParam;
    m_pThread = new MyThread(this);
}

RDThread::~RDThread()
{
    SAFE_DELETE(m_pThread);
}

void RDThread::Wait()
{
    m_pThread->wait();
}
void RDThread::Start()
{
    m_pThread->start();
}
void RDThread::RunThread()
{
   m_pCallBack(m_pCallBackParam); 
}
