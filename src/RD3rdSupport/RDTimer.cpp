// =====================================================================================
// 
//       Filename:  RDTimer.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/04/11 22:53:08
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDTimer.h"
#include "mac_define.h"
#include <QTimer>
#include "RDThread.h"
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>
#include "HMath.h"
#include <unistd.h>
QMutex mutex;

void RDTimer::TimeOutWork(void* param)
{
    RDTimer* pTimer = (RDTimer*)param;
    do{
        mutex.lock();
        pTimer->m_pWaitTime->wait(&mutex);
        pTimer->m_pCallBack(pTimer->m_pCallBackParam);
        mutex.unlock();
    }while(pTimer->m_bStart);
}

void RDTimer::CheckTime(void* param)
{
    RDTimer* pTimer = (RDTimer*)param;
    while(pTimer->m_bStart)
    {
        //double dStartTime = GetTime();
        usleep(pTimer->m_nMSec * 1000);
        pTimer->m_pWaitTime->wakeOne();
        //double oldTime = GetTime();
        //qDebug() << "timer: "<< dStartTime - oldTime;
    }
}
RDTimer::RDTimer(int msec,RDTimer_CallBack  pFun,void* param)
    :m_nMSec(msec)
{
    m_pCallBack = pFun;
    m_pCallBackParam = param;
    m_pRunThead = new RDThread(RDTimer::TimeOutWork,this);
    m_pTimeThead = new RDThread(RDTimer::CheckTime,this);
    m_pWaitTime = new QWaitCondition();
    m_bStart = false;
}
RDTimer::~RDTimer()
{
    close();
    SAFE_DELETE(m_pRunThead);
    SAFE_DELETE(m_pTimeThead);
    SAFE_DELETE(m_pWaitTime);
}

void RDTimer::start()
{
    m_bStart = true;
    m_pRunThead->Start();
    m_pTimeThead->Start();
}
void RDTimer::close()
{
    m_bStart = false;
    m_pWaitTime->wakeOne();
    m_pTimeThead->Wait();
    m_pRunThead->Wait();
}
void RDTimer::ResetTime(int msec)
{
    m_nMSec = msec;
    close();
    start();
}
