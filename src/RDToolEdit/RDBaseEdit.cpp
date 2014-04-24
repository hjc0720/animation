// =====================================================================================
// 
//       Filename:  RDBaseEdit.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/29/2011 05:56:40 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDBaseEdit.h"
#include <string>
#include "RDSpaceCell.h"
#include "RDMd5.h"
#include "RDRenderData.h"
#include "HVector4f.h"
#include "RDNode.h"
#include "RDDocument.h"
#include "RDEditerManager.h"
RDBaseEdit::RDBaseEdit(const RDMd5& md5)
    :RDBaseToolEdit(md5.GetMd5String())
    ,m_EditMd5(md5)
{
}

bool    RDBaseEdit::UpdateCommonCell(const RDMd5& pCell,const RDNode& pData)
{
    if(pCell == RDSpaceCell::GetSpaceCell()->GetCellMd5())
    {
        auto pRenderData = pData.GetRenderData(DEFAULT_RD);
        RDSpaceCell::GetSpaceCell()->SetPos(pRenderData ? pRenderData->GetPos() : pData.GetPos());
        RDSpaceCell::GetSpaceCell()->SetAngle((pRenderData ? pRenderData->GetAngle() : pData.GetAngle()) * (180/3.14));
        RDSpaceCell::GetSpaceCell()->SetScale(pRenderData ? pRenderData->GetScale() : pData.GetScale());
        return true;
    }
    return false;
}

void    RDBaseEdit::UpdateCell(const RDMd5* pCell,const RDNode& pData) 
{
    if(pCell)
    {
        UpdateCell(*pCell,pData);
        return;
    }
    int nCount = 0;
    RDCell** pCellArray = GetEditCell(nCount);
    for(int i = 0; i < nCount;i++)
        UpdateCell(pCellArray[i]->GetCellMd5(),pData);
}

bool            RDBaseEdit::UpdateCommonValue(const RDMd5& pCell,RDNode& pNode) 
{
    if(pCell == RDSpaceCell::GetSpaceCell()->GetCellMd5())
    {
        RDEditerManager& pManager = RDEditerManager::GetEditerManager();
        RDDocument* pDoc = pManager.GetDocument();
        pDoc->AddUndoCommand(new RDPosUndo(pNode));
        RDSpaceCell* pCell = RDSpaceCell::GetSpaceCell();
        MoveItemPos(pCell->GetPos(),pNode,false);
        MoveItemAngle(pCell->GetAngle() * (3.14 / 180),pNode,false);
        MoveItemScale(pCell->GetScale() ,pNode,false);
        return true;
    }
    return false;
}
