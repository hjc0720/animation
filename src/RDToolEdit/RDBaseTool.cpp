// =====================================================================================
// 
//       Filename:  RDBaseTool.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/06/11 22:21:19
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDBaseTool.h"
RDBaseTool::RDBaseTool(const QString& name)
    :RDBaseToolEdit(name)
{
    m_pActiveEdit = NULL;
    m_bEnableEdit = true;
}
bool RDBaseTool::OnMouseMove(const float3& ptScene,Qt::MouseButtons buttons,const QString& strName)
{
    if(m_pActiveEdit)
        return m_pActiveEdit->OnMouseMove(ptScene,buttons,strName);
    return false;
}
bool RDBaseTool::OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene,const QString& strName)
{
    if(m_pActiveEdit)
        return m_pActiveEdit->OnMousePress(nButtonState,ptScene,strName);
    return false;
}
bool RDBaseTool::OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ptScene,const QString& strName)
{
    if(m_pActiveEdit)
        return m_pActiveEdit->OnMouseRelease(nButtonState,ptScene,strName);
    return false;
}
bool RDBaseTool::OnKeyPress(int nKeyState)
{
    if(m_pActiveEdit)
        return m_pActiveEdit->OnKeyPress(nKeyState);
    return false;
}
