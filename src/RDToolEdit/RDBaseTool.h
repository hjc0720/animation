// =====================================================================================
// 
//       Filename:  RDBaseTool.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/06/11 22:21:24
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDBASETOOL_INC
#define  RDBASETOOL_INC

#include "RDBaseToolEdit.h"
#include <QIcon>

class RDBaseToolEdit;

class RDBaseTool : public RDBaseToolEdit
{
public:
    RDBaseTool(const QString& name);
    virtual QIcon GetToolIcon() = 0;
// =====================================================================================
// event process
    virtual bool OnMouseMove(const float3& , Qt::MouseButtons, const QString &strName);
    virtual bool OnMousePress(const Qt::MouseButtons& nButtonState, const float3& ptScene, const QString &strName);
    virtual bool OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ptScene,const std::string& strName) override;
    virtual bool OnKeyPress(int nKeyState);
protected:
    bool            m_bEnableEdit;
    RDBaseToolEdit* m_pActiveEdit;
};
#endif   // ----- #ifndef rdbasetool_INC  -----
