// =====================================================================================
// 
//       Filename:  RDSelectTool.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/07/11 22:25:39
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_SELECT_TOOL_INC
#define  RD_SELECT_TOOL_INC

#include "RDBaseTool.h"
#include "HVector3f.h"

class RDSelectTool :public RDBaseTool
{
public:
    RDSelectTool();
    virtual QIcon GetToolIcon() ;
    virtual bool OnMouseMove(const float3& ptScene,Qt::MouseButtons buttons);
    virtual bool OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene);
    virtual bool OnMouseRelease(const Qt::MouseButtons& ,const float3& ) ;
protected:
    const RDNode* HitTest(const float3& ptScene,const RDNode& pNode);
	void RefreshNodePos();
protected:
    bool m_bDrag;
    bool m_bBeginMov;
    float3 m_vStartPos;
	std::vector<float3> m_vOldPos;
};

#endif   // ----- #ifndef rdselecttool_INC  -----
