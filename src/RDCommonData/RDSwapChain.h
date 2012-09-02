// =====================================================================================
// 
//       Filename:  RDSwapChain.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/04/11 07:47:19
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include <QMutex>
class RDBuffer;

class RDSwapChain
{
public:
    RDSwapChain(unsigned int nWidth = 0,unsigned int nHeight = 0);
    ~RDSwapChain();
    void Resize(unsigned int nWidth,unsigned int nHeight);
    unsigned int GetWidth()const;
    unsigned int GetHeight()const;
    void SwapChain();
    RDBuffer* GetFrontBuffer(){return m_pFrontBuffer;}
    RDBuffer* GetBackBuffer(){return m_pBackBuffer;}
    void Lock(){m_lock.lock();}
    void UnLock(){m_lock.unlock();}
protected:
    mutable QMutex m_lock;
    RDBuffer* m_pBackBuffer;
    RDBuffer* m_pFrontBuffer;
};
