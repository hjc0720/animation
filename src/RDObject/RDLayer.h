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

enum RDLayerType
{
    RD2DLayer,
    RD3DLayer,
};

class RDLayer : public RDNode
{
public:
    RDLayer(RDLayerType nType,const QString& strName);

    virtual size_t GetChildCount()const{return  GetCameraCount() + RDNode::GetChildCount();}
    virtual RDNode* GetChild(size_t i);
    virtual const RDNode* GetChild(size_t i)const;
    virtual RDNode* GetChild(const QUuid& NodeId);
    virtual RDNode* RemoveChild(size_t i); 
    virtual void RemoveChild(const RDNode& pChild); 

    void    AddCamera(RDNode* pCameraNode){ m_vecCameraObj.push_back(pCameraNode);}
    size_t  GetCameraCount()const{return m_vecCameraObj.size();}
    const RDNode* GetCamera(size_t i)const{return m_vecCameraObj[i];}
    RDNode* GetCamera(size_t i){return m_vecCameraObj[i];}
    RDNode* RemoveCamera(size_t i);


protected:
    RDLayerType m_nType;
    std::vector<RDNode*> m_vecCameraObj;
};
#endif
