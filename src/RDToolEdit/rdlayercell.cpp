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

#include "rdlayercell.h"
#include <QCheckBox>
#include "RDMd5.h"

RDLayerCell *RDLayerCell::GetLayerCell()
{
   static RDLayerCell* layerCell = new RDLayerCell(nullptr) ;
   return layerCell;
}

const RDMd5 &RDLayerCell::GetCellMd5()
{
    const static RDMd5 layerCellMd5("RDLayerCell",-1);
    return layerCellMd5;
}

void RDLayerCell::setEnalePerspective(bool bEnable)
{
    m_pPerspective->setChecked(bEnable);
}

bool RDLayerCell::enablePerspective()
{
    return m_pPerspective->isChecked();
}

void RDLayerCell::setEnableZOrder(bool bEnable)
{
        m_pZOrder->setChecked(bEnable);
}

RDLayerCell::RDLayerCell(QWidget *parent)
    :RDCell(tr("layer"),parent)
{
    m_pZOrder = new QCheckBox(tr("z order"),this);
    AddWidget(m_pZOrder);
    connect(m_pZOrder,SIGNAL(stateChanged(int)),this,SLOT(CellChange()));
    m_pPerspective = new QCheckBox(tr("Perspective"),this);
    connect(m_pPerspective,SIGNAL(stateChanged(int)),this,SLOT(CellChange()));
    AddWidget(m_pPerspective);
}

bool RDLayerCell::enableZOrder()
{
    return m_pZOrder->isChecked();
}
