// =====================================================================================
// 
//       Filename:  RDSelectTool.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/07/11 22:25:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDSelectTool.h"
#include "RDToolManager.h"
#include "RDDocument.h"
#include "RDRenderData.h"
#include "RDObject.h"
#include <QDebug>
#include "RDNode.h"
#include "RDEditerManager.h"
#include "RDSpaceCell.h"


RDSelectTool::RDSelectTool()
    :RDBaseTool("select tool")
     ,m_bDrag(false)
     ,m_bBeginMov(false)
{

}
QIcon RDSelectTool::GetToolIcon() 
{
    return QIcon(":/select_tool");
}
bool RDSelectTool::OnMouseMove(const float3& vMov,Qt::MouseButtons buttons)
{
    RDToolManager* pManager = RDToolManager::GetToolManager();
    RDDocument* pDoc = pManager->GetDocument();
    bool bLButton = buttons.testFlag(Qt::LeftButton);
    if(bLButton && pDoc->GetSelItemCount() > 0)
    {
        if(m_bBeginMov)
        {
            RDToolManager* pManager = RDToolManager::GetToolManager();
            RDDocument* pDoc = pManager->GetDocument();
            pDoc->UndoBegin(QObject::tr("pos"));
            for(size_t i = 0; i < pDoc->GetSelItemCount(); i++)
            {
                RDNode* pData = pDoc->GetSelItem(i);
                pDoc->AddUndoCommand(new RDPosUndo(*pData));
            }
            pDoc->UndoEnd();
            m_bBeginMov = false;
        }

        float3 vNeedMov = vMov - m_vStartPos;
        vNeedMov.SetY(-vNeedMov.y());
        for(size_t i = 0; i < pDoc->GetSelItemCount(); i++)
        {
            RDNode*  pNode = pDoc->GetSelItem(i);
            MoveItemPos(vNeedMov + m_vOldPos[i],*pNode);
        }
        //m_vStartPos = vMov;
        //m_bDrag = true;
    }
    else if(bLButton && pDoc->GetSelItemCount() == 0)
    {
        m_bDrag = true;
        //drag select;
    }
    else
    {
        //HitTest;
    }
    return true;
}
bool RDSelectTool::OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene)
{
    if(!nButtonState.testFlag(Qt::LeftButton))
        return false;

    m_vStartPos = ptScene;
    m_bDrag = false;

    CancelSelectItem();
	m_vOldPos.clear();
    RDNode* pHitNode = const_cast<RDNode*>(HitTest(ptScene,*m_pFieldNode));
    if(pHitNode)
	{
        AddSelectItem(*pHitNode);
		m_vOldPos.push_back(pHitNode->GetRenderData(DEFAULT_RD)->GetPos());
	}
    m_bBeginMov = true;
    return true;
}
bool RDSelectTool::OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ) 
{
    if(nButtonState.testFlag(Qt::LeftButton))
        return false;
    m_bDrag = false;
    m_bBeginMov = false;
    return true;
}

const RDNode* RDSelectTool::HitTest(const float3& ptScene,const RDNode& pNode)
{
    if(pNode.GetObject() && pNode.GetObject()->HitTest(ptScene,pNode,DEFAULT_RD))
        return &pNode;
    for(size_t i = 0; i < pNode.GetChildCount();i++)
    {
        if(pNode.GetChild(i)->GetObject() && pNode.GetChild(i)->GetObject()->HitTest(ptScene,*pNode.GetChild(i),DEFAULT_RD))
            return pNode.GetChild(i);
        const RDNode* pHitNode = HitTest(ptScene,*pNode.GetChild(i));
        if(pHitNode)
            return pHitNode;
    }
    return NULL;
}
void RDSelectTool::RefreshNodePos()
{
	RDToolManager* pManager = RDToolManager::GetToolManager();
	RDDocument* pDoc = pManager->GetDocument();
	m_vOldPos.resize(pDoc->GetSelItemCount());
	for(size_t i = 0; i < pDoc->GetSelItemCount(); i++)
	{
		RDNode* pData = pDoc->GetSelItem(i);
		m_vOldPos[i] = pData->GetPos();
	}
}
