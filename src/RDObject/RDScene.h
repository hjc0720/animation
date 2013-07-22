// =====================================================================================
// 
//       Filename:  RDScene.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 21:53:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDSCENE_INC
#define  RDSCENE_INC
#include <QColor>
#include <QString>
#include "RDNode.h"
#include "RDResource.h"
#include <map>
#include <vector>
#include <list>

class RDSceneRenderData;
class RDFileDataStream;
class RDStory;
enum RDScene_BackType
{
    RDScene_Back_Color,
    RDScene_Back_Picture,
};

struct RDSceneData
{
    RDScene_BackType m_nBackType;
    unsigned long   backColor; //for temp use, it will be changed base on the ras algorithm.
    RDMd5*  pImage;
    bool    bStrech;
};

class RDScene: public RDNode
{
public:
    RDScene();
    RDScene(const QString& strName);
    virtual ~RDScene();
    void SetWidthHeight(int nWidth,int nHeight);
    void SetBackType(RDScene_BackType nType,const void* pData);
    const RDSceneData& GetBackData()const {return m_BackData;}
    virtual void Render(const RDTime& nTime,const QString& pRDName);
    virtual void CalFrame(const RDTime& nTime,const QString& pRDName);
    void CreateRenderData(RDRenderData& RenderData);
    //void ReleaseRenderData(RDRenderData& RenderData);
    void CreateSceneNodeMap();
    void AddSceneNodeMap(const RDNode& pAddNode);
    void RemoveSceneNodeMap(const RDNode& pRemoveNode);
    //if there is play state, trigger the next story. if there is edit state,
    //trigger the story according frame,
    bool  TriggerStory(const RDTime& nFrame,RDSceneRenderData& pSceneData);
    const RDStory* GetCurStory(const RDSceneRenderData& pSceneData)const;
    size_t GetCurStoryIndex(const RDSceneRenderData& pSceneData)const;
	size_t GetStoryCount()const{return m_StoryList.size();}
    const RDStory* GetStory(size_t nIndex)const;
    const RDStory* GetStory(const RDTime& nFrame,bool bPlay)const;
    RDStory* GetStory(size_t nIndex);
	RDTime  GetSceneLength()const;
	void RefreshStoryLength();

    virtual void AddChild(RDNode& pChild);
protected:
    void RenderImage(RDSceneRenderData& pSceneData,unsigned long nTime);
    void BlendChild(const QString& pRDName);
    RDRenderData*  CreateRenderData(const QString& pName);
protected:
    //int   m_nSceneVersion;
    RDSceneData m_BackData;
    std::map<QUuid,const RDNode*> m_NodeMap;
    std::list<RDStory*> m_StoryList;
   //friend class
    friend RDFileDataStream& operator << (RDFileDataStream& buffer,const RDScene& proj);
    friend RDFileDataStream& operator >> (RDFileDataStream& buffer,RDScene& proj);
};

//save Load operator
RDFileDataStream& operator << (RDFileDataStream& buffer,const RDScene& proj);
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDScene& proj);

#endif   // ----- #ifndef rdscene_INC  -----

