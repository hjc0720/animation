// =====================================================================================
// 
//       Filename:  RDSwapChain.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/04/11 07:47:10
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDSwapChain.h"    
#include "mac_define.h"
#include "RDBuffer.h"
#include <QMutexLocker>

RDSwapChain::RDSwapChain(unsigned int nWidth /*= 0*/,unsigned int nHeight/*  = 0 */)
{
    if(nWidth * nHeight == 0)
    {
        m_pBackBuffer = 0;
        m_pFrontBuffer = 0;
    }
    m_pBackBuffer = new RDBuffer(nWidth,nHeight);
    m_pFrontBuffer = new RDBuffer(nWidth,nHeight);
}
RDSwapChain::~RDSwapChain()
{
    SAFE_DELETE(m_pBackBuffer);
    SAFE_DELETE(m_pFrontBuffer);
}

void RDSwapChain::Resize(unsigned int nWidth,unsigned int nHeight)
{
    QMutexLocker mutex(&m_lock); 
    if(m_pBackBuffer)
        m_pBackBuffer->ResizeBuffer(nWidth,nHeight);
    else
        m_pBackBuffer = new RDBuffer(nWidth,nHeight);
    if(m_pFrontBuffer)
        m_pFrontBuffer->ResizeBuffer(nWidth,nHeight);
    else
        m_pFrontBuffer = new RDBuffer(nWidth,nHeight);
}
void RDSwapChain::SwapChain()
{
    QMutexLocker mutex(&m_lock); 
    RDBuffer* pTemp = m_pBackBuffer;
    m_pBackBuffer = m_pFrontBuffer;
    m_pFrontBuffer = pTemp;
}
unsigned int RDSwapChain::GetWidth()const
{
    QMutexLocker mutex(&m_lock); 
    if(m_pFrontBuffer)
        return m_pFrontBuffer->GetWidth();
    else
        return 0;
}
unsigned int RDSwapChain::GetHeight()const
{
    QMutexLocker mutex(&m_lock); 
    if(m_pFrontBuffer)
        return m_pFrontBuffer->GetHeight();
    else
        return 0;
}
