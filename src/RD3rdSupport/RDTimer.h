// =====================================================================================
// 
//       Filename:  RDTimer.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/04/11 22:53:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

typedef void (*RDTimer_CallBack)(void* param);
class RDThread;
class QWaitCondition;

class RDTimer
{
public:
    RDTimer(int msec,RDTimer_CallBack  pFun,void* param);
    ~RDTimer();
    void start();
    void OnTime();
    void close();
    void ResetTime(int msec);
protected:
    static void TimeOutWork(void* param);
    static void CheckTime(void* param);
protected:
    RDTimer_CallBack m_pCallBack;
    void*   m_pCallBackParam;
    int     m_nMSec;
    RDThread*  m_pRunThead;
    RDThread*  m_pTimeThead;
    QWaitCondition* m_pWaitTime;
    bool    m_bStart;
};
