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
#include "RDFileDataStream.h"
#include <cfloat>
#include <QCursor>


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

bool RDSelectTool::OnMouseMove(const float3& vMov,Qt::MouseButtons buttons,const QString&)
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
        //qDebug() << "mov offset" << vNeedMov;
        for(size_t i = 0; i < pDoc->GetSelItemCount(); i++)
        {
            RDNode*  pNode = pDoc->GetSelItem(i);
            MoveItemPos(vNeedMov + m_vOldPos[i],*pNode);
        }
        pManager->SendSceneChange();
        //m_vStartPos = vMov;
        //m_bDrag = true;
		QCursor cursor(Qt::ClosedHandCursor);
        RDToolManager::GetToolManager()->CursorChange(cursor);
    }
    else if(bLButton && pDoc->GetSelItemCount() == 0)
    {
        m_bDrag = true;
        //drag select;
    }
    else
    {
        //HitTest;
		RDNode* pHitNode = const_cast<RDNode*>(HitTest(vMov,*m_pFieldNode));
		if(pHitNode)
		{
			QCursor cursor(Qt::OpenHandCursor);
			RDToolManager::GetToolManager()->CursorChange(cursor);
		}
		else
		{
			QCursor cursor(Qt::ArrowCursor);
			RDToolManager::GetToolManager()->CursorChange(cursor);
		}
    }
    return true;
}

bool RDSelectTool::OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene,const QString&)
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

bool RDSelectTool::OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ,const std::string &)
{
    if(nButtonState.testFlag(Qt::LeftButton))
        return false;
    m_bDrag = false;
    m_bBeginMov = false;
	QCursor cursor(Qt::ArrowCursor);
	RDToolManager::GetToolManager()->CursorChange(cursor);
    return true;
}

const RDNode* RDSelectTool::HitTest(const float3& ptScene,const RDNode& pNode)
{
	float fDistance = FLT_MAX;
	const RDNode* pHitNode = nullptr;
    for(size_t i = 0; i < pNode.GetChildCount();i++)
    {
		float fTemp = pNode.GetChild(i)->HitTest(ptScene,DEFAULT_RD);
		if(fTemp > 0 && fTemp <= fDistance)
		{
			pHitNode = pNode.GetChild(i);
			fDistance = fTemp;
		}
    }
    return pHitNode;
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
