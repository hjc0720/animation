// =====================================================================================
// 
//       Filename:  RDRenderWidget.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 14:46:36
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#include "RDRenderWidget.h"
#include <QSizePolicy>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QtDebug>
#include <QCoreApplication>
#include "RDBuffer.h"
#include "RDDocument.h"
#include "mainwindow.h"
#include "RDSceneRenderData.h"
#include "RDRenderManager.h"
#include <QImage>
#include "RDBaseTool.h"
#include "RDToolManager.h"
#include "RDSpaceConvert.h"
#include <QRectF>
#include "RDEditerManager.h"
#include "RDScene.h"
#include "RDRenderDevice.h"
#include "RDLayer.h"
#include <functional>

bool g_bForceUpdate = false;

//void RDRenderWidget::setVisible(bool visible)
//{
//    QGLWidget::setVisible(visible);
//}

void RDRenderWidget::resizeGL(int w, int h)
{
    g_bForceUpdate = true;
    m_fScale = min(w / (float)m_nProjWidth,h / (float)m_nProjHeight);
    m_nXOffset = (w - GetRealPorjWidth()) / 2;
    m_nYOffset = (h - GetRealPorjHeight()) / 2;

    m_validRt.setLeft(0);
    m_validRt.setTop(0);
    m_validRt.setBottom(GetRealPorjHeight());
    m_validRt.setRight(GetRealPorjWidth());

    m_swapChain.Resize(w,h);
    m_document.SetScale(m_fScale);

    //glViewport(m_nXOffset, m_nYOffset, GetRealPorjWidth(),GetRealPorjHeight());
}

RDRenderWidget::RDRenderWidget(int nWidth, int nHeight,const QGLFormat& format,QWidget *parent /*= 0*/)
    :QGLWidget(format,parent)
     ,m_nProjWidth(nWidth)
     ,m_nProjHeight(nHeight)
     ,m_nXOffset(0)
     ,m_nYOffset(0)
     ,m_fScale(1)
     ,m_document(true)
     ,m_swapChain(nWidth,nHeight)
{
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);
    RDEditerManager::GetEditerManager().SetDocument(&m_document);
	connect(RDToolManager::GetToolManager(),SIGNAL(deleteSelItems()),this,SLOT(deleteSelItems()));
	connect(RDToolManager::GetToolManager(),SIGNAL(cursorChange(QCursor&)),this,SLOT(SetCursor(QCursor&)));
}

RDRenderWidget::~RDRenderWidget()
{
}

QSize RDRenderWidget::hintSize()const
{
    return QSize(m_nProjWidth,m_nProjHeight);
}

bool RDRenderWidget::event(QEvent* e)
{
    RDBaseTool* pCurTool = RDToolManager::GetToolManager()->GetCurTool();
    bool bProcess = false;
    switch(e->type())
    {
    case QEvent::MouseMove:
        {
            QMouseEvent* pEvent = dynamic_cast<QMouseEvent*>(e);
            bProcess = pCurTool->OnMouseMove(ClientToScene(pEvent->pos()),pEvent->buttons(),DEFAULT_RD);
        }
        break;
    case QEvent::MouseButtonPress:
        {
            QMouseEvent* pEvent = dynamic_cast<QMouseEvent*>(e);
            bProcess = pCurTool->OnMousePress(pEvent->buttons() ,ClientToScene(pEvent->pos()),DEFAULT_RD) ;
        }
        break;
    case QEvent::MouseButtonRelease:
        {
            QMouseEvent* pEvent = dynamic_cast<QMouseEvent*>(e);
            bProcess = pCurTool->OnMouseRelease(pEvent->buttons() ,ClientToScene(pEvent->pos()),DEFAULT_RD ) ;
        }
        break;
    case QEvent::KeyPress:
        {
            QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>(e);
            bProcess = pCurTool->OnKeyPress(pEvent->key()) ;
        }
        break;
    default:
        bProcess = false;
		break;
    }
    //updateGL();
	//setFocus();
    return bProcess || QWidget::event(e);
}

float3 RDRenderWidget::ClientToScene(const QPoint& pos)
{
    float3 vRet(pos.x() - m_nXOffset,pos.y() - m_nYOffset,0);
    vRet /= m_fScale;
    return vRet;
}

void RDRenderWidget::initializeGL()
{
    RDRenderDevice::CreateRenderManager(context());
}

void RDRenderWidget::paintGL()
{
    static RenderManager* pRDManager = nullptr;
    if(!pRDManager)
    {
        pRDManager = new RenderManager;
        RenderManagerCallback fun1 = std::bind(&RDRenderWidget::CalControlDirty,
                                                      this,std::placeholders::_1,std::placeholders::_2);
        RenderManagerCallback fun2 = std::bind(&RDRenderWidget::RenderControl,
                                                      this,false,std::placeholders::_1,std::placeholders::_2);
        pRDManager->RegistCallback(RDRender_SceneCalFrameEnd,fun1);
        pRDManager->RegistCallback(RDRender_SceneRenderEnd,fun2);
    }

    RDDocument* pDoc = &m_document;
    RDScene* pScene = pDoc->GetCurScene();
    if(pScene->GetMaxChangeLevel(DEFAULT_RD) == RDRender_NoChange && pDoc->GetCurTime() == pScene->GetRenderData(DEFAULT_RD)->GetTime())
        return;

    glScissor(0, 0,size().width() ,size().height());
    RDRenderDevice::GetRenderManager()->ClearScreen(float4(0.5,0.5,0.5,1),1,RDClearColor);
    glViewport(m_nXOffset, m_nYOffset, GetRealPorjWidth(),GetRealPorjHeight());
    glScissor(m_nXOffset, m_nYOffset, GetRealPorjWidth(),GetRealPorjHeight());
    RDRenderDevice::GetRenderManager()->ClearScreen(float4(0,0,0,0),1,RDClearColor | RDClearDepth | RDClearStencil);
    pRDManager->SetRenderName(DEFAULT_RD);
    pRDManager->SetScene(pScene);
    QPointF pt(m_nXOffset,m_nYOffset);
    if(!pRDManager->RenderScene(pt,m_validRt,pDoc->GetCurTime()))
        return;
    //qDebug() << "On Time :" << pDoc->GetCurTime();
}

bool RDRenderWidget::CalControlDirty(RDNode *pScene,RenderManager* man)
{
    qDebug() << "CalControlDirty";
    RDBaseTool* pCurTool = RDToolManager::GetToolManager()->GetCurTool();
    pScene->GetRenderData(man->getRenderName())->UnionDirty(pCurTool->GetDirtyRect());
    pCurTool->resetDirty();
    return true;
}

bool RDRenderWidget::RenderControl(bool bDepth,RDNode *,RenderManager *)
{
    qDebug() << "CalControlDirty";
    RDBaseTool* pCurTool = RDToolManager::GetToolManager()->GetCurTool();
    if(bDepth)
        pCurTool->OnDrawNoDepth();
    else
        pCurTool->OnDrawInDepth();
    return true;
}

void RDRenderWidget::deleteSelItems() 
{
	qDebug() << "delete all select Items";
	for(size_t i = 0; i < m_document.GetSelItemCount(); i++)
		emit DelNoded(*m_document.GetSelItem(i));
	m_document.DelSelItems();
	RDEditerManager::GetEditerManager().UpdateProperty(0);
	update();
}
