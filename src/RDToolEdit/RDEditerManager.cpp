// =====================================================================================
// 
//       Filename:  RDEditerManager.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 11:53:17 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDEditerManager.h"
#include "mac_define.h"
#include "RDImageEdit.h"
#include "RDCell.h"
#include <QDebug>
#include "RDRenderData.h"
#include "RDObject.h"
#include "RDNode.h"

RDEditerManager* RDEditerManager::m_pEditerManager = NULL;

RDEditerManager& RDEditerManager::GetEditerManager()
{
    if(!m_pEditerManager)
    {
        m_pEditerManager = new RDEditerManager();
    }
    return *m_pEditerManager;
}
void RDEditerManager::ReleaseEditerManager()
{
    SAFE_DELETE(m_pEditerManager);
}
void RDEditerManager::UpdateProperty(RDNode* pData)
{
    if(m_pCurNode == pData)
        return;
    m_pCurNode = pData;

    int nCount = 0;;
    RDCell** pCellArray = 0;
    if(pData)
    {
        RDBaseEdit* pEdit = m_EditList[pData->GetObject()->GetObjMd5()];
        if(pEdit)
            pCellArray = pEdit->GetEditCell(nCount);
        UpdateCell(0,*pData);
    }
    emit  PropertyChanged(pCellArray,nCount);
}
RDEditerManager::RDEditerManager()
{
    RegisterEditer();
    RegisterCell();
}

RDEditerManager::~RDEditerManager()
{
}

void RDEditerManager::RegisterEditer()
{
    RDBaseEdit* pNewEdit = new RDImageEdit(RDMd5("RDImageObject"));
    m_EditList[pNewEdit->GetEditerMd5()] = pNewEdit;
}

void RDEditerManager::RegisterCell()
{
    for(auto it = m_EditList.begin(); it != m_EditList.end(); it++)
    {
        RDBaseEdit* pEdit = it->second;
        int nCellCount = 0;
        RDCell** pCellArray = pEdit->GetEditCell(nCellCount);
        for(int i = 0; i < nCellCount; i++)
        {
            m_CellList[pCellArray[i]->GetCellMd5()] = pCellArray[i];
            connect(pCellArray[i], SIGNAL(CellChanged(const RDMd5& )), this, SLOT(CellChange(const RDMd5& )));
        }
    }
}

void    RDEditerManager::UpdateCell(const RDMd5* pCell,const RDNode& pData)
{
    RDBaseEdit* pEdit = m_EditList[pData.GetObject()->GetObjMd5()];
    if(!pEdit)
        return;
    pEdit->UpdateCell(pCell,pData);
}

void    RDEditerManager::CellChange(const RDMd5& pCell)
{
    if(!m_pCurNode)
        return;
    RDBaseEdit* pEdit = m_EditList[m_pCurNode->GetObject()->GetObjMd5()];
    if(!pEdit)
        return;
    pEdit->UpdateValue(pCell,*m_pCurNode);
    emit SceneChange();
}
