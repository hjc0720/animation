// =====================================================================================
// 
//       Filename:  RDImageEdit.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/03/2011 10:24:31 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDIMAGEEDIT_INC
#define  RDIMAGEEDIT_INC

#include "RDBaseEdit.h"
class RDMd5;
class RDRenderData;
class RDImageEdit : public RDBaseEdit
{
public:
    RDImageEdit(const RDMd5& md5);
    virtual RDCell** GetEditCell(int& nCount) const;
    void    UpdateValue(const RDMd5& pCell,RDNode& pData) ;
protected:
    bool    UpdateCell(const RDMd5& pCell,const RDNode& pData);
};
#endif   // ----- #ifndef rdimageedit_INC  -----
