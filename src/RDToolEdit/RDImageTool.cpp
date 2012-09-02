// =====================================================================================
// 
//       Filename:  RDImageTool.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  28/06/11 21:20:49
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDImageTool.h"
#include "HMath.h"
#include <QFileDialog>
#include "RDResourceManager.h"
#include "RDImageObject.h"
#include "RDNode.h"
#include "RDDocument.h"
#include "RDToolManager.h"

RDImageTool::RDImageTool()
    :RDBaseTool("image tool")
{
    m_bDragged = false;
    m_nImageWidth = 0;
    m_nImageHeight = 0;
}
QIcon RDImageTool::GetToolIcon() 
{
    return QIcon(":/image_tool");
}
bool RDImageTool::OnMouseMove(const float3& ,Qt::MouseButtons buttons)
{
    if(buttons.testFlag(Qt::LeftButton))
    {
        m_bDragged = true;
        return true;
    }
    return false;
}
bool RDImageTool::OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene)
{
    if(!nButtonState.testFlag(Qt::LeftButton))
        return false;
    m_bDragged = false;
    m_nImageWidth = 0;
    m_nImageHeight = 0;
    m_vImagePos = ptScene;
    return true;
}
bool RDImageTool::OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ptScene) 
{
    static int nObjIndex = 1; 
    if(nButtonState.testFlag(Qt::LeftButton) || !m_bDragged)
        return false;
    m_bDragged = false;

    m_nImageWidth = floatToInt(ptScene.x() - m_vImagePos.x());
    m_nImageHeight = floatToInt(m_vImagePos.y() - ptScene.y() );

    if(m_nImageWidth < 0 )
    {
        m_nImageWidth = -m_nImageWidth;
        m_vImagePos.SetX(m_vImagePos.x() - m_nImageWidth);
    }
    if(m_nImageHeight < 0 )
    {
        m_nImageHeight = -m_nImageHeight;
        m_vImagePos.SetY(m_vImagePos.y() + m_nImageHeight);
    }

    QString fileName = QFileDialog::getOpenFileName(NULL,QObject::tr("Get Picture"));
    if(fileName.isEmpty())
        return false;

    RDImageObject* pNewObject = new RDImageObject( fileName);
    pNewObject->SetWidth(m_nImageWidth);
    pNewObject->SetHeight(m_nImageHeight);
    QString imageName( QString(QObject::tr("Image %1")).arg(nObjIndex));
    RDNode* pNewNode = new RDNode(imageName,m_vImagePos,pNewObject);
	pNewNode->SetParent(m_pFieldNode);
	pNewObject->SetNode(pNewNode);
    AddChild(*m_pFieldNode,*pNewNode);
    RDToolManager::GetToolManager()->StopCurTool();
    return true;
}
