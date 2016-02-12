// =====================================================================================
// 
//       Filename:  RDToolBar.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/11 21:23:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDToolBar.h"
#include "RDToolManager.h"
#include "RDBaseTool.h"
#include <QAction>
#include <QActionGroup>
#include "mac_define.h"
#include <QDebug>
#include "mainwindow.h"

RDToolBar::RDToolBar(QWidget* parent ,bool bSubTool )
    :QToolBar(parent)
{
    m_pMainWindow = dynamic_cast<MainWindow*>(parent);
    setMovable(false);
    setIconSize(QSize(32,32));
    m_pGroup = new QActionGroup(this);
    m_pGroup->setExclusive(true);
    if(!bSubTool)
    {
        bool bFirst = true;
        RDToolManager* pToolManger = RDToolManager::GetToolManager();
        for(RDToolIt it = pToolManger->GetBeginTool();it != pToolManger->GetEndTool(); it++)
        {
            RDBaseTool* pTool = it->second;
            QAction* pAction = addAction(pTool->GetToolIcon(),pTool->GetName().data());
            pAction->setObjectName(pTool->GetName().data());
            pAction->setActionGroup(m_pGroup);
            pAction->setCheckable(true);        
            if(bFirst)
            {
                pAction->setChecked(true);
                pToolManger->SwitchTool(pTool->GetName(),*m_pMainWindow->GetCurDocument());
                bFirst = false;
            }
        }
        //addSeparator();
        connect(m_pGroup,SIGNAL(triggered(QAction*)),this,SLOT(OnChangeTool(QAction*)));
        connect(pToolManger,SIGNAL(ChangeTool(const std::string& )),this,SLOT(OnTrigTool(const std::string& )));
    }
}

RDToolBar::~RDToolBar()
{
    SAFE_DELETE(m_pGroup);
}
void RDToolBar::OnChangeTool(QAction* pAction)
{
    qDebug()<< "switch tool" << pAction->objectName();
    RDToolManager::GetToolManager()->SwitchTool(pAction->objectName().toStdString(),*m_pMainWindow->GetCurDocument());
}
void RDToolBar::OnTrigGroupTool()
{

}
void RDToolBar::OnTrigTool(const std::string& pToolName)
{
    qDebug() << "trig tool slot";
    QAction* pAction = FindToolAction(pToolName.data());
    if(pAction)
        pAction->setChecked(true);
}
QAction* RDToolBar::FindToolAction(const QString& pToolName)
{
    QList<QAction*>  ActionList = actions();
    for (int i = 0; i < ActionList.size(); ++i) {
        if (ActionList.at(i)->objectName() == pToolName)
        {
            return ActionList[i];
        }
    }
    return NULL;
}
