// =====================================================================================
// 
//       Filename:  RDBuffer.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/04/11 22:42:59
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDBUFFER_INC
#define  RDBUFFER_INC
typedef unsigned int uint;
class QImage;
class QRectF;

class RDBuffer
{
public:
    RDBuffer(unsigned int nWidth,unsigned int nHeight);
    RDBuffer(const QImage& pImage);
    ~RDBuffer();
    void ResizeBuffer(unsigned int nWidth,unsigned int nHeight);
    void Clear();
    void FillColor(unsigned int dwColor);
    void Draw(const QRectF& dstRt,const RDBuffer& buffer,const QRectF& srcRt);
    unsigned int GetWidth()const;
    unsigned int GetHeight()const;
    QImage* GetBuffer(){return m_pBuffer;}
protected:
    QImage* m_pBuffer;
};
#endif   // ----- #ifndef rdbuffer_INC  -----
