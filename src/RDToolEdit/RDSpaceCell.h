// =====================================================================================
// 
//       Filename:  RDSpaceCell.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/04/2011 09:45:54 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  RDPOSCELL_INC
#define  RDPOSCELL_INC
#include "RDCell.h"

class float3;
class QLineEdit;
class RDSpaceCell :public RDCell
{
public:
    static RDSpaceCell* GetSpaceCell();
    virtual const RDMd5& GetCellMd5();
    void    SetPos(const float3& newPos);
    void    GetPos(float3& newPos);
protected:
    RDSpaceCell(QWidget* parent);
    static RDSpaceCell* m_pCell ;
protected:
    QLineEdit* m_pPosX ;
    QLineEdit* m_pPosY ;
    QLineEdit* m_pPosZ ;
};
#endif   // ----- #ifndef rdposcell_INC  -----
