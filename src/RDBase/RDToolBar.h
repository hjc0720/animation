// =====================================================================================
// 
//       Filename:  RDToolBar.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/07/11 21:23:50
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDTOOLBAR_INC
#define  RDTOOLBAR_INC

#include <QToolBar>
class QWidget;
class QActionGroup;
class MainWindow;

class RDToolBar : public QToolBar
{
    Q_OBJECT
public:
    RDToolBar(QWidget* parent = 0,bool bSubTool = false);
    ~RDToolBar();
protected:
    QAction* FindToolAction(const QString& pToolName);
public slots:
    //switch tool;
    void OnChangeTool(QAction* pAction);
//trig a group tool ,just for sub tool bar;
    void OnTrigGroupTool();
    void OnTrigTool(const std::string &pToolName);
protected:
    QActionGroup* m_pGroup;
    MainWindow*   m_pMainWindow;
};

#endif   // ----- #ifndef rdtoolbar_INC  -----
