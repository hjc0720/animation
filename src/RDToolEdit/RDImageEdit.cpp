// =====================================================================================
// 
//       Filename:  RDImageEdit.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/03/2011 10:24:48 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDImageEdit.h"
#include "RDSpaceCell.h"
#include "RDImageCell.h"
#include "RDImageObject.h"
#include "RDNode.h"

RDImageEdit::RDImageEdit(const RDMd5& md5)
    :RDBaseEdit(md5)
{
}

RDCell** RDImageEdit::GetEditCell(int& nCount) const
{
    static RDCell* pCellArray[] = {
        RDImageCell::GetImageCell(),
        RDSpaceCell::GetSpaceCell()
    };
    nCount = sizeof(pCellArray) / sizeof(RDCell*);
    return pCellArray;
}

bool    RDImageEdit::UpdateCell(const RDMd5& pCell,const RDNode& pData)
{
    if(pCell == RDImageCell::GetImageCell()->GetCellMd5())
    {
        const RDImageObject* pImage = dynamic_cast<const RDImageObject*>( pData.GetObject());
        if(!pImage)
            return false;
        RDImageCell* pCell = RDImageCell::GetImageCell();
        pImage->Lock();
        pCell->SetImageFileName(pImage->GetFile());
        pCell->SetImageOriginSize(pImage->GetOriginWidth(),pImage->GetOriginHeight());
        pCell->SetImageSize(pImage->GetWidth(),pImage->GetHeight());
        pImage->UnLock();
        return true;
    }
    return UpdateCommonCell(pCell,pData);
}

void    RDImageEdit::UpdateValue(const RDMd5& pCell,int nIndex,RDNode& pData) 
{
    if(pCell == RDImageCell::GetImageCell()->GetCellMd5())
    {
        RDImageObject* pImage = dynamic_cast<RDImageObject*>( pData.GetObject());
        if(!pImage)
            return;
        RDImageCell* pCell = RDImageCell::GetImageCell();
        pImage->Lock();
        if(nIndex == RDImageFileChange)
        {
            pImage->SetFile(pCell->GetImageFileName());
            pData.SetChangeLevel(RDRender_GraphicChange);
            pCell->SetImageOriginSize(pImage->GetOriginWidth(),pImage->GetOriginHeight());
        }
        else if(nIndex == RDImageSizeChange) 
        {
            pImage->SetWidth(pCell->GetImageWidth());
            pImage->SetHeight(pCell->GetImageHeight());
            pData.SetChangeLevel(RDRender_TransChange);
        }
        pImage->UnLock();
        return;
    }
    UpdateCommonValue(pCell,nIndex,pData);
}
