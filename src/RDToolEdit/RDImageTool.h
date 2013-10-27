// =====================================================================================
// 
//       Filename:  RDImageTool.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/06/11 21:20:59
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDIMAGETOOL_INC
#define  RDIMAGETOOL_INC
#include "RDBaseTool.h"
#include "HVector3f.h"

class RDImageTool : public RDBaseTool
{
public:
    RDImageTool();
    virtual QIcon GetToolIcon() ;

    virtual bool OnMouseMove(const float3& ptScene, Qt::MouseButtons buttons, const QString &);
    virtual bool OnMousePress(const Qt::MouseButtons& nButtonState, const float3& ptScene, const QString &);
    virtual bool OnMouseRelease(const Qt::MouseButtons& , const float3& , const QString &strName) ;
protected:
    int     m_nImageWidth;
    int     m_nImageHeight;
    bool    m_bDragged;
    float3  m_vImagePos;
};
#endif   // ----- #ifndef rdimagetool_INC  -----
