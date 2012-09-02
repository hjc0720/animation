// =====================================================================================
// 
//       Filename:  RDBaseToolEdit.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/06/11 21:09:41
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDBaseToolEdit.h"
#include "RDToolManager.h"
#include "RDDocument.h"
#include "RDEditerManager.h"
#include "RDRenderData.h"
#include "RDObject.h"
#include "RDNode.h"
#include <QDebug>
#include "RDSpaceCell.h"

RDBaseToolEdit::RDBaseToolEdit(const QString& name)
    :m_ToolEditName(name)
{
    m_pFieldNode = NULL;
}
bool RDBaseToolEdit::BeginEdit(RDNode* pFieldNode)
{
    qDebug() << "Begin " << m_ToolEditName << "edit";
    m_pFieldNode = pFieldNode;
    return true;
}
void RDBaseToolEdit::EndEdit()
{
    qDebug() << "end " << m_ToolEditName << "edit";
    m_pFieldNode = NULL;
}
void   RDBaseToolEdit::AddSelectItem(RDNode& selItem)
{
    RDToolManager* pManager = RDToolManager::GetToolManager();
    RDDocument* pDoc = pManager->GetDocument();
    pDoc->AddSelItem(selItem);
    RDEditerManager::GetEditerManager().UpdateProperty(&selItem);
}
void   RDBaseToolEdit::CancelSelectItem()
{
    RDToolManager* pManager = RDToolManager::GetToolManager();
    RDDocument* pDoc = pManager->GetDocument();
    pDoc->ClearSelItem();
    RDEditerManager::GetEditerManager().UpdateProperty(0);
}
void   RDBaseToolEdit::AddChild(RDNode& pParent,RDNode& pChild)
{
    RDDocument* pDoc = RDToolManager::GetToolManager()->GetDocument();
    pDoc->AddChildNode(pParent,pChild);
    RDEditerManager::GetEditerManager().UpdateProperty(&pChild);
	emit RDEditerManager::GetEditerManager().AddNode(pChild);
}
void   RDBaseToolEdit::MoveItemPos(const float3& vNewPos,RDNode& pNode)
{
    RDToolManager* pManager = RDToolManager::GetToolManager();
    RDDocument* pDoc = pManager->GetDocument();
    RDEditItemType nType = pDoc->GetEditType();
    pNode.Lock();
    RDRenderData* pRenderData = pNode.GetRenderData(DEFAULT_RD);
    switch(nType)
    {
    case RDEdit_ModifyItem:
        pNode.MovePos(vNewPos - pRenderData->GetPos());
        break;
    case RDEdit_AutoKey:
        //qDebug() << "add key doc frame" << pDoc->GetCurFrame();
        pNode.AddPosKey(pDoc->GetCurTime(),vNewPos - pNode.GetPos());
        break;
    default:
        pNode.UnLock();
        return;
    }
    pNode.SetChangeLevel(RDRender_TransChange);
    pNode.UnLock();
    RDEditerManager::GetEditerManager().UpdateCell(&RDSpaceCell::GetSpaceCell()->GetCellMd5(),pNode);
}
