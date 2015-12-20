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

#ifndef RDLAYERCELL_H
#define RDLAYERCELL_H

#include "RDCell.h"

class QCheckBox;

class RDLayerCell : public RDCell
{
    Q_OBJECT
public:
    static RDLayerCell* GetLayerCell();
    virtual const RDMd5& GetCellMd5();
    void setEnableZOrder(bool bEnable);
    bool enableZOrder();
    void setEnalePerspective(bool bEnable);
    bool enablePerspective();

protected:
    RDLayerCell(QWidget* parent);
protected:
    QCheckBox* m_pZOrder;
    QCheckBox* m_pPerspective;
};

#endif // RDLAYERCELL_H
