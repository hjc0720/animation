// =====================================================================================
// 
//       Filename:  RDRenderData.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/04/11 20:53:36
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDRENDERDATA_INC
#define  RDRENDERDATA_INC
#include "RDBuffer.h"
#include <vector>
#include "mac_define.h"
#include <QRectF>
#include "HVector3f.h"

enum RDRenderChangeLevel
{
    RDRender_NoChange,
    //RDRender_ReRender,
    RDRender_TransChange,
    RDRender_GraphicChange,
    RDRender_New,
};

class RDObject;
class RDSceneRenderData;
class RDNode;
class RDSection;
class RDStory;

class RDRenderPrivateData
{

};

class RDRenderData 
{
public:
    RDRenderData(RDNode& node,const RDSceneRenderData& SceneRenderData);
    virtual         ~RDRenderData();
    RDRenderChangeLevel GetChangeLevel()const{return m_nChangeLevel;}
    void            SetChangeLevel(RDRenderChangeLevel nLevel);
    void            ResetChangeLevel(){m_nChangeLevel = RDRender_NoChange;}
    RDRenderChangeLevel  GetRenderChangeLevel()const{return m_nRenderChangeLevel;}
    void            SetRenderChangeLevel(RDRenderChangeLevel nLevel);
    void            ResetRenderChangeLevel(){m_nRenderChangeLevel = RDRender_NoChange;}

    void            SetPrivateData(RDRenderPrivateData* pPrivateData){m_pPrivateData = pPrivateData;}
    RDRenderPrivateData*        GetPrivateData(){return m_pPrivateData ;}
    const RDRenderPrivateData*  GetPrivateData()const{return m_pPrivateData ;}

    const RDObject* GetObject()const;
    RDObject*       GetObject();
    const RDNode&   GetNode()const{return m_Node;}
    RDNode&         GetNode(){return m_Node;}
    RDRenderData*   GetParent(){return m_pParent;}
    const RDTime&   GetTime()const;
    const RDTime&   GetSectionTime()const{return m_nSectionTime;}
    void            SetSectionTime(const RDTime& nSectionTime){m_nSectionTime = nSectionTime;}
    virtual float   GetScale()const;
    virtual void    SetScale(float fScale);

    const RDSceneRenderData& GetSceneRD()const{return  m_SceneRenderData;}

    RDSection*      GetCurSection(){return m_pCurSection;}
    const RDStory*  GetCurStory()const;
    void            SetCurSection(RDSection* pSection){m_pCurSection = pSection;}
    bool            IsPlay()const;
    uint            GetSceneWidth();
    uint            GetSceneHeight();
    float           GetSceneNowWidth();
    float           GetSceneNowHeight();
    const QRectF&   GetBound()const{return m_rtBound;}
    QRectF          GetScaleBound()const;
    void            SetBound(const QRectF& bound){m_rtBound = bound;}
    const QRectF&   GetDirty()const{return m_rtDirty;}
    void            UnionDirty(const QRectF& dirty){m_rtDirty |= dirty;}
    void            ResetDirty(){m_rtDirty.setRect(0,0,0,0);}

    //space info
    const float3&    GetPos()const { return m_vPos; }
    void             SetPos(const float3& vPos){m_vPos = vPos;}
protected:
    void Lock()const;
    void UnLock()const;
public:
    RDBuffer            m_RenderBuffer;
    RDBuffer            m_ItemBuffer;
protected:
    RDRenderChangeLevel m_nChangeLevel;
    RDRenderChangeLevel m_nRenderChangeLevel;
    RDNode&             m_Node;
    RDRenderPrivateData*m_pPrivateData;
    RDSection*          m_pCurSection;
    RDRenderData*       m_pParent;
    const RDSceneRenderData&  m_SceneRenderData;
    RDTime             m_nSectionTime;
    QRectF              m_rtDirty;
    QRectF              m_rtBound;
    float3              m_vPos;
};

inline QRectF RDRenderData::GetScaleBound()const
{
    return QRectF(m_rtBound.left() * GetScale(),
            m_rtBound.top() * GetScale(),
            m_rtBound.width() * GetScale(),
            m_rtBound.height() * GetScale());
}
#endif   // ----- #ifndef rdrenderdata_INC  -----
