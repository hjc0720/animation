// =====================================================================================
// 
//       Filename:  RDSceneRenderData.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/11 21:11:23
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDSCENERENDERDATA_INC
#define  RDSCENERENDERDATA_INC
#include "RDRenderData.h"
#include "HMath.h"
#include "mac_define.h"

class RDSceneRenderData : public RDRenderData
{
public:
    RDSceneRenderData(RDNode& obj);
    RDSceneRenderData(int nWidth,int nHeight,RDNode& obj);
    virtual void SetScale(float fScale);
    virtual float GetScale()const{ return m_fScale;}
    void    SetWidthHeight(int nWidth,int nHeight){m_nWidth = nWidth;m_nHeight = nHeight;}

    uint     GetWidth()const{return m_nWidth;}
    uint     GetHeight()const {return m_nHeight;}
    float   GetNowWidth()const{return (m_nWidth * m_fScale);}
    float   GetNowHeight()const{return (m_nHeight * m_fScale);}
    bool    IsPlay()const{return m_bPlay;}

    const RDTime& GetTime()const{return m_nCurFrame ;}
    void SetTime(const RDTime& nTime){m_nCurFrame = nTime ;}

protected:
    bool    m_bPlay;
    uint    m_nWidth;
    uint    m_nHeight;
    float   m_fScale;
    RDTime m_nCurFrame;
};

#endif   // ----- #ifndef rdscenerenderdata_INC  -----
