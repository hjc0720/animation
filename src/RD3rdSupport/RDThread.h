// =====================================================================================
// 
//       Filename:  RDThread.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  14/04/11 21:16:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
typedef void (*RDThread_CallBack)(void* param);

class MyThread;
class RDThread
{
public:
    RDThread(RDThread_CallBack pCallBack,void* pCallBackParam);
    ~RDThread();
    void RunThread();
    void Start();
    void Wait();
protected:
    MyThread* m_pThread;
    RDThread_CallBack m_pCallBack;
    void*  m_pCallBackParam;

};
