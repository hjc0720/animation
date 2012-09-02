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

RDImageEdit::RDImageEdit(const RDMd5& md5)
    :RDBaseEdit(md5)
{

}
RDCell** RDImageEdit::GetEditCell(int& nCount) const
{
    static RDCell* pCellArray[] = {RDSpaceCell::GetSpaceCell()};
    nCount = sizeof(pCellArray) / sizeof(RDCell*);
    return pCellArray;
}
bool    RDImageEdit::UpdateCell(const RDMd5& pCell,const RDNode& pData)
{
    return UpdateCommonCell(pCell,pData);
}
void    RDImageEdit::UpdateValue(const RDMd5& pCell,RDNode& pData) 
{
    UpdateCommonValue(pCell,pData);
}
