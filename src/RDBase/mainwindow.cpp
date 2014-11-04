#include "mainwindow.h"
#include <QStatusBar>
#include "RDDocument.h"
#include "mac_define.h"
#include "RDRenderWidget.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QtDebug>
#include "RDProject.h"
#include "RDSetSceneDlg.h"
#include "RDSceneRenderData.h"
#include "RDScene.h"
#include "RDResourceManager.h"
#include "RDToolBar.h"
#include "RDToolManager.h"
#include "RDPropertySheet.h"
#include "RDEditerManager.h"
#include "RDTimelineView.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),m_bInit(false)
{
    InitData();
    InitGUI();
}

MainWindow::~MainWindow()
{
    //release gui
    ReleaseGUI();
    //release data
    ReleaseData();
}

void MainWindow::InitGUI()
{
    m_pCenterWidget = NULL;
    m_pBar = NULL;
	m_pTimeLineView = 0;
}
void MainWindow::InitData()
{
    QString m_strCachePath = QDir::homePath();
    m_strCachePath += "/.Animation/temp_cache/";
    RDResourceManager::GetResourceManager()->SetResPath(m_strCachePath);

    //RDDocument* pDocument = new RDDocument(true);
    //m_vecDocList.push_back(pDocument);
    //m_nCurDocument = 0;
}

void MainWindow::setVisible(bool visible)
{
    if(!m_bInit && visible)
        InitWindow();
   QMainWindow::setVisible(visible); 
}

void MainWindow::InitWindow()
{
    AddCenterWidget();
    AddMenuBar();
    AddStatusBar();
    OnReloadTimeLine(*GetCurDocument()->GetCurScene());
    AddToolBar();
    AddEditorBar();
    AddProperty();

    connect(RDToolManager::GetToolManager(),SIGNAL(SceneChange()),m_pCenterWidget,SLOT(updateGL()));
    connect(&RDEditerManager::GetEditerManager(),SIGNAL(SceneChange()),m_pCenterWidget,SLOT(updateGL()));
    resize(800,600);
	m_UndoGroup.setActiveStack(GetCurDocument()->GetUndoStack());
    m_bInit = true;
}

void MainWindow::AddCenterWidget()
{
    int nWidth = QApplication::desktop()->width();
    int nHeight = QApplication::desktop()->height();
    QGLFormat format;
    format.setVersion(4,3);
    format.setProfile(QGLFormat::CoreProfile);
    m_pCenterWidget = new RDRenderWidget(nWidth,nHeight,format,this);
	setCentralWidget(m_pCenterWidget);
	RDDocument& pDoc = m_pCenterWidget->GetDocument();
	m_UndoGroup.addStack(pDoc.GetUndoStack());
}
void MainWindow::AddOtherWidget()
{

}
void MainWindow::OnSave()
{
    m_pBar->showMessage(tr("save"));
    qDebug() << "BeginSave" << endl;
    if(GetCurDocument()->bHavePath())
    {
        qDebug() << "Save have path" << GetCurDocument()->GetProjPath();
        GetCurDocument()->SaveProj();
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this,QString(tr("File Name")));
        if(!fileName.isEmpty())      
            GetCurDocument()->SaveProjAs(fileName);
    }
}

void MainWindow::OnSetScene()
{
    if(GetCurDocument()->bOpenProj())
    {
        RDSetSceneDlg dlg;
        RDScene* pScene = GetCurDocument()->GetCurScene();
        dlg.SetValue(pScene->GetBackData());
        if(dlg.exec() == QDialog::Accepted)
        {
            if(dlg.GetValue().m_nBackType == RDScene_Back_Color)
                pScene->SetBackType(RDScene_Back_Color,&dlg.GetValue().backColor);   
            else if(dlg.GetValue().m_nBackType == RDScene_Back_Picture)
                pScene->SetBackType(RDScene_Back_Picture,dlg.GetValue().pImage);   
        }
    }
}
void MainWindow::OnLoad()
{
    m_pBar->showMessage(tr("load"));
    qDebug() << "BeginLoad" << endl;
    QString fileName = QFileDialog::getOpenFileName(this,QString(tr("File Name")));
    if(!fileName.isEmpty())      
    {
        //RDProject proj(720,576,50);
        GetCurDocument()->LoadProj(fileName);
    }
    UpdateEditTool();
	OnReloadTimeLine(*GetCurDocument()->GetCurScene());
}
void MainWindow::AddMenuBar()
{
    QMenuBar* pMenuBar = menuBar();
    //file menu
    QMenu* pFileMenu = pMenuBar->addMenu(tr("&File"));
    QAction* pSaveAction = pFileMenu->addAction(tr("&Save"));
    connect(pSaveAction,SIGNAL(triggered()),this,SLOT(OnSave()));
    if(!GetCurDocument()->bOpenProj())
       pSaveAction->setEnabled(false);
    QAction* pLoadAction = pFileMenu->addAction(tr("&Load"));
    connect(pLoadAction,SIGNAL(triggered()),this,SLOT(OnLoad()));

    //edit menu
    QMenu* pEditMenu = pMenuBar->addMenu(tr("&Edit"));
    QAction* pSetScene = pEditMenu->addAction(tr("&Setting Scene"));
    connect(pSetScene,SIGNAL(triggered()),this,SLOT(OnSetScene()));

	QAction* undoAction = m_UndoGroup.createUndoAction(this,tr("Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);
	pEditMenu->addAction(undoAction);

	QAction* redoAction = m_UndoGroup.createRedoAction(this,tr("Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);
	pEditMenu->addAction(redoAction);

    connect(undoAction, SIGNAL(triggered(bool)), m_pCenterWidget, SLOT(updateGL()));
    connect(redoAction, SIGNAL(triggered(bool)), m_pCenterWidget, SLOT(updateGL()));
}
void MainWindow::AddStatusBar()
{
    m_pBar = statusBar();
    m_pBar->showMessage(tr("Ready"));
}
void MainWindow::AddToolBar()
{
    addToolBar(Qt::LeftToolBarArea,new RDToolBar(this));
}
void MainWindow::AddEditorBar()
{

}
void MainWindow::ReleaseGUI()
{
    SAFE_DELETE(m_pBar);
    SAFE_DELETE(m_pCenterWidget);
}
void MainWindow::ReleaseData()
{
    //for(size_t i = 0; i < m_vecDocList.size();i++)
    //{
        //RDDocument* pDocument = m_vecDocList[i];
        //SAFE_DELETE(pDocument);
    //}
    RDResourceManager::ReleaseResourceManager();
}
RDDocument* MainWindow::GetCurDocument()
{
    return &m_pCenterWidget->GetDocument();
}
void MainWindow::UpdateEditTool()
{
    RDToolManager::GetToolManager()->SwitchTool(NULL,*GetCurDocument());
}
void MainWindow::AddProperty()
{
    m_pProperty = new RDPropertySheet(360,this);
    addDockWidget(Qt::RightDockWidgetArea,m_pProperty);
    connect(&RDEditerManager::GetEditerManager(), SIGNAL(PropertyChanged(RDCell** ,int )), m_pProperty, SLOT(PropertyChanged(RDCell** ,int )));
}
void MainWindow::OnReloadTimeLine(RDScene& pScene)
{
	if(m_pTimeLineView)
		m_pTimeLineView->deleteLater();
	m_pTimeLineView = new RDTimelineView(pScene,this);
    connect(&RDEditerManager::GetEditerManager(),SIGNAL(AddNoded(RDNode&)),m_pTimeLineView,SLOT(InsertObj(RDNode&)));
    connect(m_pCenterWidget,SIGNAL(DelNoded(RDNode&)),m_pTimeLineView,SLOT(DelObj(RDNode&)));
    connect(m_pTimeLineView,SIGNAL(FrameChanged(const RDTime& )),this,SLOT(OnFrameChanged(const RDTime&)));
    connect(m_pTimeLineView,SIGNAL(SectionChanged()),m_pCenterWidget,SLOT(updateGL()));

    connect(m_pTimeLineView,SIGNAL(addStory()),this,SLOT(OnAddStory()));
    connect(m_pTimeLineView,SIGNAL(removeCurStory()),this,SLOT(OnDeleteCurStory()));
    connect(m_pTimeLineView,SIGNAL(switchStory(int)),this,SLOT(OnSwitchStory(int)));
    addDockWidget(Qt::BottomDockWidgetArea,m_pTimeLineView);
}

void MainWindow::OnFrameChanged(const RDTime& nTime)
{
    GetCurDocument()->SetCurTime(nTime);
    m_pCenterWidget->updateGL();
}

void MainWindow::OnAddStory()
{
	int nIndex = GetCurDocument()->GetCurScene()->GetStoryCreatIndex();
	QString defaultName(tr("story "));
	defaultName += QString::number(nIndex);
	bool ok;
	defaultName = QInputDialog::getText(this, tr("Add New Story"),
			tr("Story Name:"), QLineEdit::Normal,
			defaultName, &ok);
	if (ok && !defaultName.isEmpty())
	{
		GetCurDocument()->AddStoryAndTrigger(defaultName.toStdString());
		m_pTimeLineView->updateStory();
	}
}

void MainWindow::OnSwitchStory(int nIndex)
{
	GetCurDocument()->TriggerStory(nIndex);
	m_pTimeLineView->trigStory(nIndex);
}

void MainWindow::OnDeleteCurStory()
{
	int nIndex = GetCurDocument()->GetCurScene()->GetCurStoryIndex(DEFAULT_RD);
	if(!GetCurDocument()->RemoveStoryAndTrigger(nIndex))
		QMessageBox::warning(this,tr("Delete Story Error"),tr("This is last story, you can't delete it!"));
	else
		m_pTimeLineView->updateStory();
}
