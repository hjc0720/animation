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
#include "HVector4f.h"


class RDImageTool : public RDBaseTool
{
public:
    RDImageTool();
    virtual QIcon GetToolIcon() ;

    virtual bool OnMouseMove(const float3& ptScene, Qt::MouseButtons buttons, const QString &)override;
    virtual bool OnMousePress(const Qt::MouseButtons& nButtonState, const float3& ptScene, const QString &)override;
    virtual bool OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ptScene,const std::string& strName) override;
protected:
    void setRect();

protected:
    int     m_nImageWidth;
    int     m_nImageHeight;
    bool    m_bDragged;
    float3  m_vImagePos;
    float2  m_fLeftTop;


    // RDBaseToolEdit interface
public:
    virtual QRectF GetDirtyRect(const std::string& strName)override;
    virtual void OnDrawNoDepth(const std::string &strName)override;
};
#endif   // ----- #ifndef rdimagetool_INC  -----
