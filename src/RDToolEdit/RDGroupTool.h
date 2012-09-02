// =====================================================================================
// 
//       Filename:  RDGroupTool.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/07/11 07:45:48
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef  RDGROUPTOOL_INC
#define  RDGROUPTOOL_INC

#include <vector>
#include "RDBaseTool.h"

class RDGroupTool : public RDBaseTool
{
    virtual int GetSubToolCount(){return m_vSubTool.size();} 
    virtual QIcon GetSubIcon(int nIndex) {return m_vSubTool[nIndex]->GetToolIcon();}
protected:
    std::vector<RDBaseTool*> m_vSubTool;
};
#endif   // ----- #ifndef rdgrouptool_INC  -----
