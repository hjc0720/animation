/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rdlayeredit.h"
#include "rdlayercell.h"
#include "RDLayer.h"
#include <typeinfo>
#include <QDebug>

RDLayerEdit::RDLayerEdit(const RDMd5 &md5)
    :RDBaseEdit(md5)
{
}

RDCell **RDLayerEdit::GetEditCell(int &nCount) const
{
    static RDCell* pCellArray[]={
        RDLayerCell::GetLayerCell()
    };
    nCount = sizeof(pCellArray) / sizeof(RDCell*);
    return pCellArray;
}

void RDLayerEdit::UpdateValue(const RDMd5 &pCell, int , RDNode &pData)
{
    if(pCell == RDLayerCell::GetLayerCell()->GetCellMd5())
    {
        try{
            RDLayer& layer = dynamic_cast<RDLayer&>(pData);
            RDLayerCell* pCell = RDLayerCell::GetLayerCell();
            layer.setZOrder(pCell->enableZOrder());
            layer.setPerspective(pCell->enablePerspective());
        }
        catch(std::bad_cast)
        {
            qFatal("RDLayerEdit::UpdateValue: RDLayer dynamic_cast error!");
            return;
        }
    }
}

bool RDLayerEdit::UpdateCell(const RDMd5 &pCell, const RDNode &pData)
{
    if(pCell == RDLayerCell::GetLayerCell()->GetCellMd5())
    {
        try{
            const RDLayer& layer = dynamic_cast<const RDLayer&>(pData);
            RDLayerCell* pCell = RDLayerCell::GetLayerCell();
            pCell->setEnableZOrder(layer.isZOrder());
            pCell->setEnalePerspective(layer.isPerspective());
        }
        catch(std::bad_cast)
        {
            qFatal("RDLayerEdit::UpdateValue: RDLayer dynamic_cast error!");
            return false;
        }

        return true;
    }
    return false;
}
