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

#ifndef RDLAYEREDIT_H
#define RDLAYEREDIT_H

#include "RDBaseEdit.h"
class RDLayerEdit : public RDBaseEdit
{
public:
    RDLayerEdit(const RDMd5 &md5);
    virtual RDCell** GetEditCell(int& nCount) const;
    void    UpdateValue(const RDMd5& pCell, int, RDNode& pData) ;
    bool    UpdateCell(const RDMd5& pCell,const RDNode& pData) ;
};

#endif // RDLAYEREDIT_H
