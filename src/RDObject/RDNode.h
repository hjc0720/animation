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
#include <QString>
#include <QUuid>
#include <QMutex>
#include "mac_define.h"
#include <list>
#include <map>
#include <QUndoCommand>
#include "RDRenderData.h"
#include "HVector4f.h"

class RDObject;
class RDRenderData;
class RDFileDataStream;
class RDSection;

typedef std::vector<RDSection*>::iterator RDSectionList;
class RDScene;

class RDNode
{
public:
    RDNode();
    RDNode(const QString& strName);
    RDNode(const QString& strName,const float3& pos,RDObject* pObj);
    virtual ~RDNode();
    void Lock(){ m_lock.lock();}
    void UnLock(){m_lock.unlock();}

    const QString& GetName()const {return m_strName;}
    const QUuid& GetNodeID()const{return m_NodeID;}

    const float3& GetPos()const{return m_vPos;}
    void MovePos(const float3& vOffset){ m_vPos += vOffset;}
    void SetPos(const float3& vPos){ m_vPos = vPos;}

    bool AddSection(const RDTime& nStoryTime,const RDTime& nLength,const QUuid& storyId);
	RDTime GetSectionMaxLength(const QUuid& idStory)const;
	void AddPosKey(const RDTime& nFrame,const float3& vOffsetPos );

    RDObject* GetObject(){return m_pObj;}
    const RDObject* GetObject()const{return m_pObj;}
    void SetObject(RDObject* pObj){m_pObj = pObj;}

    virtual void Render(const RDTime& nTime,const QString& pRDName) ;
    virtual void CalFrame(const RDTime& nTime,const QString& pRDName) ;


    //child operation function
    size_t GetTotalChildCount()const;
    virtual size_t GetChildCount()const{return m_vecChildObj.size();}
    virtual void AddChild(RDNode& pChild){m_vecChildObj.push_back(&pChild);pChild.m_pParent = this;}
    virtual RDNode* GetChild(size_t i){return m_vecChildObj[i];}
    virtual const RDNode* GetChild(size_t i)const{return m_vecChildObj[i];}
    virtual RDNode* GetChild(const QUuid& NodeId);
    virtual RDNode* RemoveChild(size_t i); 
    virtual void RemoveChild(const RDNode& pChild); 

    RDNode* GetParent(){return m_pParent;}
    const RDNode* GetParent()const{return m_pParent;}
    void SetParent(RDNode* pParent){m_pParent = pParent;}

    RDRenderData*   GetRenderData(const QString& pName);
    const RDRenderData*   GetRenderData(const QString& pName)const;
    void            RemoveRenderData();
    void            SetRenderScale(float fScale,const QString& pName);

    RDRenderChangeLevel GetMaxChangeLevel(const QString& pName)const;
    RDRenderChangeLevel GetMaxRenderChangeLevel(const QString& pName)const;
    void SetChangeLevel(RDRenderChangeLevel nLevel);
    RDRenderChangeLevel GetChangeLevel(const QString& pName)const;
    RDRenderChangeLevel GetRenderChangeLevel(const QString& pName)const;

    size_t GetSectionCount(const QUuid& idStory)const;
    RDSection* GetSection(const QUuid& idStory,size_t nIndex);
protected:
    void            MoveSection(const RDTime& nSteps, RDSectionList pStart,RDSectionList pEnd );
    void            UpdateSection(const RDTime& nFrame /*global frame*/,RDRenderData& pRD);
    RDSection*      GetLastSectionBefore(size_t nCurStoryIndex);
    RDSection*      GetSection(const QUuid& nStoryId,const RDTime& nStoryFrame);
    virtual RDRenderData*  CreateRenderData(const QString& pName);
    RDScene*        GetSceneNode();
    bool            CalSpaceVector(const RDTime& nFrame,RDRenderData& RenderData);
    void            CalNodeMatrix(RDRenderData& RenderData);
protected:
    float3      m_vPos;
    float3      m_vScale;
    QString     m_strName;
    QUuid       m_NodeID;
    std::vector<RDNode*> m_vecChildObj;
    std::map<QUuid,std::vector<RDSection*>> m_vecSetctionListMap;
    std::map<QString,RDRenderData*> m_vecRenderData;
    //std::list<RDParagraph*> m_vecParagraph;

    RDNode*     m_pParent;
    RDObject*   m_pObj;

    mutable QMutex m_lock;

    friend RDFileDataStream& operator << (RDFileDataStream& buffer,const RDNode& proj);
    friend RDFileDataStream& operator >> (RDFileDataStream& buffer,RDNode& proj);
};

RDFileDataStream& operator << (RDFileDataStream& buffer,const RDNode& proj);
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDNode& proj);

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
