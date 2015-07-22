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
#include <QDebug>
#include "RDFileDataStream.h"
#include "rdline.h"
#include <QRectF>

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
bool RDImageTool::OnMouseMove(const float3& ptScene,Qt::MouseButtons buttons,const QString&)
{
    if(buttons.testFlag(Qt::LeftButton))
    {
        m_bDragged = true;
        m_nImageWidth = floatToInt(abs(ptScene.x() - m_vImagePos.x()));
        m_nImageHeight = floatToInt(abs(ptScene.y() - m_vImagePos.y()));
        m_fLeftTop.x = std::min(ptScene.x(),m_vImagePos.x());
        m_fLeftTop.y = std::min(ptScene.y(),m_vImagePos.y());
        setRect();
        m_pFieldNode->SetChangeLevel(RDRender_TransChange);
        RDToolManager::GetToolManager()->SendSceneChange();
        return true;
    }
    return false;
}
bool RDImageTool::OnMousePress(const Qt::MouseButtons& nButtonState,const float3& ptScene,const QString& )
{
    if(!nButtonState.testFlag(Qt::LeftButton))
        return false;
    m_bDragged = false;
    m_nImageWidth = 0;
    m_nImageHeight = 0;
    m_vImagePos = ptScene;

    setRect();
    return true;
}

bool RDImageTool::OnMouseRelease(const Qt::MouseButtons& nButtonState,const float3& ptScene,const std::string& strName)
{
    static int nObjIndex = 1; 
    if(nButtonState.testFlag(Qt::LeftButton) || !m_bDragged)
        return false;

    m_bDragged = false;    

    QString fileName = QFileDialog::getOpenFileName(NULL,QObject::tr("Get Picture"));
    if(fileName.isEmpty())
    {
        m_pFieldNode->SetChangeLevel(RDRender_TransChange);
        RDToolManager::GetToolManager()->SendSceneChange();
        return false;
    }

    m_nImageWidth = floatToInt(abs(ptScene.x() - m_vImagePos.x()));
    m_nImageHeight = floatToInt(abs(ptScene.y() - m_vImagePos.y()));
    m_vImagePos = (ptScene + m_vImagePos) * 0.5f;
    RDSpaceParam param = m_pFieldNode->GetEditSpaceParam(strName,&m_pFieldNode->GetNodeMatrix(strName));
    float3 vPos = param.Convert2DTo3D(m_vImagePos,param.GetZValue());

    RDImageObject* pNewObject = new RDImageObject( fileName);
    pNewObject->SetWidth(m_nImageWidth);
    pNewObject->SetHeight(m_nImageHeight);
    QString imageName( QString(QObject::tr("Image %1")).arg(nObjIndex));
    qDebug() << vPos;
    RDNode* pNewNode = new RDNode(imageName.toStdString(),vPos,pNewObject);
	pNewNode->SetParent(m_pFieldNode);
	pNewObject->SetNode(pNewNode);
    AddChild(*m_pFieldNode,*pNewNode);
    m_pFieldNode->SetChangeLevel(RDRender_TransChange);
    RDToolManager::GetToolManager()->StopCurTool();
    nObjIndex ++;
    return true;
}

void RDImageTool::setRect()
{
    if(!m_pLine)
        m_pLine = new RDLine(1);
    m_pLine->clear();
    m_pLine->setColor(0xffff0000);
    m_pLine->addRect(QRectF((m_fLeftTop.x), (m_fLeftTop.y),static_cast<float>(m_nImageWidth),static_cast<float>(m_nImageHeight)));
}


QRectF RDImageTool::GetDirtyRect(const std::string& strName)
{
    float fScale = m_pFieldNode->GetRenderData(strName)->GetSceneScale();
    //return QRectF(m_vImagePos.x() * fScale, m_vImagePos.y() * fScale,m_nImageWidth * fScale,m_nImageHeight * fScale);
    return QRectF(0,0,1920,1080);
}

void RDImageTool::OnDrawNoDepth(const std::string& strName)
{
    if(!m_pLine || !m_bDragged)
        return;
    matrix4x4 mat;
    float fWidth = m_pFieldNode->GetRenderData(strName)->GetSceneWidth();
    float fHeight = m_pFieldNode->GetRenderData(strName)->GetSceneHeight();
    matrix4x4::CreateOrthoMat(mat,0,fWidth,0,fHeight,-1,1);
    m_pLine->drawLine(mat);
}
