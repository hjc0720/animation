// =====================================================================================
// 
//       Filename:  RDBuffer.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/04/11 22:42:50
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDBuffer.h"
#include <QImage>
#include <QPainter>
#include <cstdlib>
#include <algorithm>
#include <QColor>
#include "mac_define.h"
using namespace std;

#define ARGB_A(color) ((((unsigned int)color) & 0xff000000) >> 24)
#define ARGB_R(color) ((((unsigned int)color) & 0x00ff0000) >> 16)
#define ARGB_B(color) ((((unsigned int)color) & 0x0000ff00) >> 8)
#define ARGB_G(color) ((((unsigned int)color) & 0x000000ff) )
RDBuffer::RDBuffer(const QImage& pImage)
{
    m_pBuffer = new QImage(pImage);
}
RDBuffer::RDBuffer(unsigned int nWidth,unsigned int nHeight)
{
    m_pBuffer = NULL;
    if(nWidth * nHeight > 0)
        m_pBuffer = new QImage(nWidth,nHeight,QImage::Format_ARGB32);
}
RDBuffer::~RDBuffer()
{
    SAFE_DELETE(m_pBuffer);
}

void RDBuffer::FillColor(unsigned int dwColor)
{
    m_pBuffer->fill(dwColor);
}
void RDBuffer::ResizeBuffer(unsigned int nWidth,unsigned int nHeight)
{
    if(m_pBuffer)
    {
        if(nWidth != (unsigned int)m_pBuffer->width() || nHeight != (unsigned int)m_pBuffer->height())
            SAFE_DELETE(m_pBuffer);
    }
    if(!m_pBuffer)
        m_pBuffer = new QImage(nWidth,nHeight,QImage::Format_ARGB32);
}
void RDBuffer::Clear()
{
    m_pBuffer->fill(0);
}
void RDBuffer::Draw(const QRectF& dstRt,const RDBuffer& buffer,const QRectF& srcRt)
{
    QPainter painter(m_pBuffer);
    painter.drawImage(dstRt,*buffer.m_pBuffer,srcRt);
}
unsigned int RDBuffer::GetWidth()const
{
    return m_pBuffer->width();
}
unsigned int RDBuffer::GetHeight()const
{
    return m_pBuffer->height();
}
