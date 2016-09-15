// 
//       Filename:  RDNode.h
// 
//    Description:  node info,about space pos in scene and animation info;
// 
//        Version:  1.0
//        Created:  27/06/11 07:38:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDNODE_INC
#define  RDNODE_INC
#include <vector>
#include <QUuid>
#include "mac_define.h"
#include <list>
#include <map>
#include <QUndoCommand>
#include "RDRenderData.h"
#include "HVector4f.h"
#include <QRectF>
#include "RDSpaceConvert.h"
#include <mutex>
#include <string>

class RDObject;
class RDRenderData;
class RDJsonDataStream;
class RDSection;
class RDLayer;

typedef std::vector<RDSection*>::iterator RDSectionList;
typedef std::lock_guard<std::mutex> RDSingleLock;

class RDScene;
class RDCamera;
class RDMd5;

namespace Json {
class Value;
}

enum RDClipboardType
{
    RDKeyValue,
};

class RDNode
{
public:
    RDNode() = default;
    RDNode(const std::string& strName);
    RDNode(const std::string& strName,const float3& pos,RDObject* pObj);
    virtual ~RDNode();
    void Lock(){ m_lock.lock();}
    void UnLock(){m_lock.unlock();}

    virtual void Serialize(RDJsonDataStream& buffer, Json::Value &parent);

    const std::string& GetName()const {return m_strName;}
    const QUuid& GetNodeID()const{return m_NodeID;}
    bool    collapse()const{return m_bCollapse;}
    void    setCollapse(bool bCollapse){m_bCollapse = bCollapse;}
    bool    isParentCollapse()const;
    virtual const RDMd5 &getNodeMd5()const;

    //static pos
    const float3& GetPos()const{return m_vPos;}
    void MovePos(const float3& vOffset){ m_vPos += vOffset;}
    void SetPos(const float3& vPos){ m_vPos = vPos;}
    //angle
    const float3& GetAngle()const{return m_vAngle;}
    void MoveAngle(const float3& vOffset){m_vAngle += vOffset;}
    void SetAngle(const float3& vAngle){m_vAngle = vAngle;}
    //scale
    const float3& GetScale()const{return m_vScale;}
    void MoveScale(const float3& vOffset){m_vScale.Mul(vOffset);}
    void SetScale(const float3& vScale){m_vScale = vScale;}

    //dynamic
    const float3& GetDynamicPos(const std::string& pName)const;

    bool AddSection(const RDTime& nStoryTime,const RDTime& nLength,const QUuid& storyId);
	void AddPosKey(const RDTime& nTime,const float3& vOffsetPos ,const std::string& strName);
	void AddAngleKey(const RDTime& nTime,const float3& vOffsetAngle ,const std::string& strName);
	void AddScaleKey(const RDTime& nTime,const float3& vOffsetScale ,const std::string& strName);

    RDObject* GetObject(){return m_pObj;}
    const RDObject* GetObject()const{return m_pObj;}
    void SetObject(RDObject* pObj){m_pObj = pObj;}

    virtual void Render(const RDTime& nTime,const std::string& pRDName) ;
    virtual void CalFrame(const RDTime& nTime,const std::string& pRDName) ;
	//击中测试，返回到近平面的距离，如果小于等于０，没有击中
    float HitTest(const float3& vScenePt,const std::string& RDName)const;

    //child operation function
    size_t GetTotalChildCount()const;
    virtual size_t GetChildCount()const{return m_vecChildObj.size();}
    virtual void AddChild(RDNode& pChild){m_vecChildObj.push_back(&pChild);pChild.m_pParent = this;}
    virtual RDNode* GetChild(size_t i){return m_vecChildObj[i];}
    virtual const RDNode* GetChild(size_t i)const{return m_vecChildObj[i];}
    const RDNode* GetChild(const QUuid& NodeId)const;
    RDNode* GetChild(const QUuid& NodeId);
    virtual RDNode* RemoveChild(size_t i); 
    void RemoveChild(const RDNode& pChild); 

    RDNode* GetParent(){return m_pParent;}
    const RDNode* GetParent()const{return m_pParent;}
    void SetParent(RDNode* pParent){m_pParent = pParent;}

    RDRenderData*   GetRenderData(const std::string& pName);
    const RDRenderData*   GetRenderData(const std::string& pName)const;
    void            RemoveRenderData();
    RDRenderPrivateData* GetPrivateData(const std::string& pName)const;

    RDRenderChangeLevel GetMaxChangeLevel(const std::string& pName)const;
    RDRenderChangeLevel GetMaxRenderChangeLevel(const std::string& pName)const;
    void SetChangeLevel(RDRenderChangeLevel nLevel);
    RDRenderChangeLevel GetChangeLevel(const std::string& pName)const;
    RDRenderChangeLevel GetRenderChangeLevel(const std::string& pName)const;

    size_t GetSectionCount(const QUuid& idStory)const;
    RDSection* GetSection(const QUuid& idStory,size_t nIndex);
    RDSection* GetCurSection(const std::string& pName)const{return GetRenderData(pName)->GetCurSection();}
	void RemoveSection(RDSection* pSection);

    virtual const matrix4x4&     GetViewProjMat(const std::string& RDName);
    RDCamera* GetCamera(const std::string& strName) const;
    RDSpaceParam GetEditSpaceParam(const std::string &strName, const matrix4x4 *pWorldMat)const;
    virtual QRectF GetSceneRt(const std::string& strName)const;
    const matrix4x4& GetNodeMatrix(const std::string &strName)const;

    virtual void paste(const std::string&);
protected:
    virtual void    CalChildFrame(const RDTime& nTime,const std::string& pRDName);
    RDTime          calSectionTime(RDTime time,RDRenderData& pRD);
    void            MoveSection(const RDTime& nSteps, RDSectionList pStart,RDSectionList pEnd );
    RDTime          UpdateSection(const RDTime& nFrame /*global frame*/,RDRenderData& pRD);
	RDSection*		FindNearestSection(RDTime& storyTime,const RDRenderData& renderData,RDTime time);
    RDSection*      GetSection(const QUuid& nStoryId,const RDTime& nStoryFrame);
    virtual RDRenderData*  CreateRenderData(const std::string& pName);
    const RDScene *GetSceneNode()const;
    const RDLayer*        GetLayerNode()const;
    bool            CalSpaceVector(const RDTime& nFrame,RDRenderData& RenderData);
    void            CalNodeMatrix(RDRenderData& RenderData);
	RDTime GetSectionMaxLength(const QUuid& idStory)const;
protected:
    bool        m_bCollapse = false;
    float3      m_vPos;
    float3      m_vAngle;
    float3      m_vScale=float3(1,1,1);
    std::string m_strName;
    QUuid       m_NodeID=QUuid::createUuid();
    std::vector<RDNode*> m_vecChildObj;
    std::map<QUuid,std::vector<RDSection*>> m_vecSetctionListMap;
    std::map<std::string,RDRenderData*> m_vecRenderData;

    RDNode*     m_pParent = nullptr;
    RDObject*   m_pObj = nullptr;

    mutable std::mutex m_lock;

    friend RDJsonDataStream& operator << (RDJsonDataStream& buffer,const RDNode& proj);
    friend RDJsonDataStream& operator >> (RDJsonDataStream& buffer,RDNode& proj);
};

//undo
class RDUndoCommand 
{
public:
    RDUndoCommand(const QUuid& NodeId,QString str = QString()):m_NodeId(NodeId),m_strText(str){ m_pNode = 0;}
    virtual ~RDUndoCommand(){}
    void SetNode(RDNode* pNode){m_pNode = pNode;}
    const QUuid GetNodeID()const{return m_NodeId;}
    const QString& GetText()const{return m_strText;}
    virtual void undo() = 0;
    virtual void redo() = 0;
protected:
    QUuid   m_NodeId;
    QString m_strText;
    RDNode* m_pNode;
};

class RDPosUndo :public RDUndoCommand
{
public:
    RDPosUndo(RDNode& pNode);
    void undo();
    void redo();
protected:
    float3  m_OldPos;
};
#endif   // ----- #ifndef rdnode_INC  -----
