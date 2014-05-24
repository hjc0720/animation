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
    static RDSpaceCell& GetSpaceCell();
    virtual const RDMd5& GetCellMd5();
    void            SetPos(const float3& newPos);
    const float3&   GetPos()const;

    void            SetAngle(const float3& newAngle);
    const float3&   GetAngle()const;

    void            SetScale(const float3& newScale);
    const float3&   GetScale()const;
protected:
    virtual int GetCurChangeIndex(){return -1;};
    RDVec3Widget*  CreateVectorWidget(const QString& name,double fMin,double fMax,int decimals,double step);
    RDSpaceCell(QWidget* parent);
protected:
    RDVec3Widget* m_pPos;
    RDVec3Widget* m_pAngle;
    RDVec3Widget* m_pScale;
};
#endif   // ----- #ifndef rdposcell_INC  -----
