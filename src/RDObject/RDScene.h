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
#include "RDNode.h"
#include "RDResource.h"
#include <map>
#include <vector>
#include <list>
#include <QRectF>

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
    RDScene(const std::string& strName);
    virtual ~RDScene();
    virtual void Serialize(RDFileDataStream& buffer,bool bSave);
    void SetWidthHeight(int nWidth,int nHeight);
    void SetBackType(RDScene_BackType nType,const void* pData);
    const RDSceneData& GetBackData()const {return m_BackData;}

    virtual void Render(const RDTime& nTime,const std::string& pRDName);
    virtual void CalFrame(const RDTime& nTime,const std::string& pRDName)override;

    void CreateRenderData(RDRenderData& RenderData);

    void CreateSceneNodeMap();
    void AddSceneNodeMap(const RDNode& pAddNode);
    void RemoveSceneNodeMap(const RDNode& pRemoveNode);

	//story
    //if there is play state, trigger the next story. if there is edit state,
    //trigger the story according frame,
	size_t AddStory(const std::string& strName);
	size_t AddStory(RDStory* pStory);
	bool RemoveStory(size_t nIndex);
	int GetStoryCreatIndex()const{return m_nStoryCreateIndex;}
    bool  TriggerStory(int nStoryIndex,RDTime nFrame,RDSceneRenderData& pSceneData);
    const RDStory& GetCurTrigStory(const std::string& name)const;
    const RDStory& GetCurStory(const std::string& name)const;
    size_t GetCurStoryIndex(const std::string& name)const;
    size_t GetCurStoryIndex(const RDSceneRenderData& pSceneData)const;
	size_t GetStoryCount()const{return m_StoryList.size();}
    const RDStory* GetStory(size_t nIndex)const{return m_StoryList[nIndex];}
    const RDStory* GetTrigStory(RDTime nFrame,const std::string& pRDName)const;

	void RefreshStoryLength();

    void            setRenderScale(float fScale,const std::string& pName);

    virtual void AddChild(RDNode& pChild);

    virtual QRectF GetSceneRt(const std::string& strName) const;
protected:
    void RenderImage(RDSceneRenderData& pSceneData,unsigned long nTime);
    RDRenderData*  CreateRenderData(const std::string& pName)override;
protected:
    RDSceneData m_BackData;
    std::map<QUuid,const RDNode*> m_NodeMap;
    std::vector<RDStory*> m_StoryList;
    int         m_nStoryCreateIndex = 0;
};
#endif   // ----- #ifndef rdscene_INC  -----

