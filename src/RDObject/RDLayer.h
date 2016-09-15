/*
 * =====================================================================================
 *
 *       Filename:  RDLayer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/02/13 20:40:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _RD_LAYER_H
#define _RD_LAYER_H
#include "RDNode.h"

class RDCamera;
class RDLight;
class RDLayerRenderData;
class RDUBO;
class RDLayer : public RDNode
{
public:
    RDLayer(){};
    RDLayer(const std::string& strName);
    virtual ~RDLayer();
    virtual void Serialize(RDJsonDataStream& buffer, Json::Value &parent)override;

    virtual size_t GetChildCount()const{return  GetCameraCount() + GetLightCount() + RDNode::GetChildCount();}
    virtual RDNode* GetChild(size_t i);
    virtual const RDNode* GetChild(size_t i)const;
    virtual RDNode* RemoveChild(size_t i); 

    //camera function
    void    AddCamera(RDCamera* pCameraNode){ m_vecCameraObj.push_back(pCameraNode);}
    RDCamera* RemoveCamera(size_t i);
    size_t  GetCameraCount()const{return m_vecCameraObj.size();}
    const RDCamera* GetCamera(size_t i)const{return m_vecCameraObj[i];}
    RDCamera* GetCamera(size_t i){return m_vecCameraObj[i];}

    RDCamera*   GetCurCamera(const std::string& pRDName) const;
    void        SetCurCamera(const std::string& pRDName,size_t nIndex);

    //light function
    void AddLight(RDLight* pLight){m_vecLight.push_back(pLight);}
    RDLight* RemoveLight(size_t nIndex);
    size_t GetLightCount()const{return m_vecLight.size();}
    const RDLight* GetLight(size_t i)const{return m_vecLight[i];}
    RDLight*    GetLight(size_t i){return m_vecLight[i];}

    RDUBO*      GetLightParam(const std::string& name)const;

    void        setZOrder(bool bEnable){m_bZOrder = bEnable;}
    void        setPerspective(bool bEnable){m_bPerspective = bEnable;}

    bool        isZOrder()const{return m_bZOrder;}
    bool        isPerspective()const{return m_bPerspective;}
protected:
    RDCamera* GetCurCamera(const RDLayerRenderData& pLayerRD) const;
    float2      CalObjMinMax(const std::string& pRDName);
    virtual RDRenderData*  CreateRenderData(const std::string &pName)override;

    virtual void    CalChildFrame(const RDTime& nTime,const std::string& pRDName)override;

protected:
    bool m_bZOrder;
    bool m_bPerspective;
    std::vector<RDCamera*> m_vecCameraObj;
    std::vector<RDLight*> m_vecLight;

    // RDNode interface
public:
    virtual void Render(const RDTime &nTime, const std::string &pRDName)override;
};
#endif
