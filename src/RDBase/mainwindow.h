#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <vector>
#include <QUndoGroup>
#include "mac_define.h"

class RDDocument;
class RDRenderWidget;
class RDResourceManager;
class QStatusBar;
class QDockWidget;
class RDTimelineView;
class RDScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitWindow();
    virtual void setVisible(bool visible);
    RDDocument* GetCurDocument();//{return m_vecDocList[m_nCurDocument];}
protected:
    void InitGUI();
    void InitData();
    void ReleaseGUI();
    void ReleaseData();
    void UpdateEditTool();
//windows layout
    virtual void AddCenterWidget();
    virtual void AddOtherWidget();
    virtual void AddMenuBar();
    virtual void AddStatusBar();
    virtual void AddToolBar();
    virtual void AddEditorBar();
    virtual void AddProperty();
    //Menu operate function
public slots:
    void OnSave();
    void OnLoad();
    void OnSetScene();
	void OnReloadTimeLine(RDScene& pScene);
    void OnFrameChanged(const RDTime& nFrame);
	void OnAddStory();
	void OnDeleteCurStory();
	void OnSwitchStory(int nIndex);
	void OnSectionChange();
protected:
    bool        m_bInit;
    RDRenderWidget* m_pCenterWidget;
    QDockWidget*    m_pProperty;
    QUndoGroup      m_UndoGroup;
    QStatusBar* m_pBar;
	RDTimelineView* m_pTimeLineView;
};

#endif // MAINWINDOW_H
