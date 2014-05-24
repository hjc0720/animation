// =====================================================================================
// 
//       Filename:  RDBaseEdit.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/29/2011 05:56:43 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDBASEEDIT_INC
#define  RDBASEEDIT_INC

#include "RDBaseToolEdit.h"
#include "RDMd5.h"

class RDCell;
class RDBaseEdit : public RDBaseToolEdit
{
public:
    RDBaseEdit(const RDMd5& md5);
    ~RDBaseEdit();
    const RDMd5& GetEditerMd5()const{return m_EditMd5;};
    virtual RDCell** GetEditCell(int& nCount)const = 0;
    void            UpdateCell(const RDMd5* pCell,const RDNode& pData) ;
    virtual void    UpdateValue(const RDMd5& pCell,int nIndex,RDNode& pData) = 0;
protected:
    virtual bool    UpdateCell(const RDMd5& pCell,const RDNode& pData) = 0;
    bool            UpdateCommonCell(const RDMd5& pCell,const RDNode& pData);
    bool            UpdateCommonValue(const RDMd5& pCell,int nIndex,RDNode& pNode) ;
    bool            UpdateSpaceValue(RDNode& pNode) ;
private:
    RDMd5 m_EditMd5;
};
#endif   // ----- #ifndef rdbaseedit_INC  -----
