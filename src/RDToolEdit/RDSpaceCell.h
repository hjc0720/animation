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
class RDVec3Widget;
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
    RDVec3Widget* m_pPos;
};
#endif   // ----- #ifndef rdposcell_INC  -----
