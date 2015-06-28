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
#include <list>
#include "RDStory.h"

class RDSceneRenderData : public RDRenderData
{
public:
    RDSceneRenderData(const std::string& name,RDNode& obj);
    RDSceneRenderData(const std::string& name,int nWidth,int nHeight,RDNode& obj);
    void SetSceneScale(float fScale);
    virtual float GetSceneScale()const override{ return m_fScale;}
    virtual RenderManager* getRenderManager()const override;
    virtual void setRenderManager(RenderManager*) override;

    void    SetWidthHeight(int nWidth,int nHeight){m_nWidth = nWidth;m_nHeight = nHeight;}

    uint     GetWidth()const{return m_nWidth;}
    uint     GetHeight()const {return m_nHeight;}
    float   GetNowWidth()const{return (m_nWidth * m_fScale);}
    float   GetNowHeight()const{return (m_nHeight * m_fScale);}
    bool    IsPlay()const{return m_bPlay;}

    const RDTime& GetTime()const{return m_nCurFrame ;}
    void SetTime(const RDTime& nTime){m_nCurFrame = nTime ;}

    size_t GetCurStoryIndex()const{return m_nCurStoryIndex;}
    void SetCurStoryIndex(size_t nIndex){m_nCurStoryIndex = nIndex;} 
    void trigStory(const RDStory& story,RDTime nFrame);
    const RDStory& GetCurTrigStory()const{return m_trigStory.front();}
    const RDStory& GetStory(RDTime nTime)const;
	void removeTrigStory(const QUuid& storyID);
	const std::list<RDStory>& getTrigStoryList()const{return m_trigStory;}
protected:
    bool    m_bPlay;
    uint    m_nWidth;
    uint    m_nHeight;
    float   m_fScale;
    RDTime m_nCurFrame;
    RenderManager* m_pRenderManager;

    size_t m_nCurStoryIndex;
    std::list<RDStory> m_trigStory;                                              
};

#endif   // ----- #ifndef rdscenerenderdata_INC  -----
