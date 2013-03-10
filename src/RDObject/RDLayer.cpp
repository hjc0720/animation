/*
 * =====================================================================================
 *
 *       Filename:  RDLayer.cpp
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
#include "RDLayer.h"

RDLayer::RDLayer(RDLayerType nType,const QString& strName)
    :RDNode(strName)
     ,m_nType(nType)
{
     m_strName = strName;
}

RDNode* RDLayer::GetChild(size_t i)
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    return RDNode::GetChild(i - m_vecCameraObj.size());
}

const RDNode* RDLayer::GetChild(size_t i)const
{
    if(i < m_vecCameraObj.size())
        return GetCamera(i);
    return RDNode::GetChild(i);
}

RDNode* RDLayer::GetChild(const QUuid& NodeId)
{
    for(auto it = m_vecCameraObj.begin(); it != m_vecCameraObj.end(); it++)
    {
        if((*it)->GetNodeID() == NodeId)
            return *it;
        auto ret = (*it)->GetChild(NodeId);
        if(ret)
            return ret;
    }
    return RDNode::GetChild(NodeId);
}

RDNode* RDLayer::RemoveChild(size_t i)
{
    if(i < GetCameraCount())
        return RemoveCamera(i);

    return RDNode::RemoveChild(i - m_vecCameraObj.size());
} 

void RDLayer::RemoveChild(const RDNode& pChild)
{
    for(auto it = m_vecCameraObj.begin(); it != m_vecCameraObj.end(); it++)
    {
        if(*it == &pChild)
        {
            m_vecCameraObj.erase(it);
            return;
        }
    }
    RDNode::RemoveChild(pChild);
} 


RDNode* RDLayer::RemoveCamera(size_t i)
{
    RDNode* pRemove = m_vecCameraObj[i];
    m_vecCameraObj.erase(m_vecCameraObj.begin() + i);
    return pRemove;
}
