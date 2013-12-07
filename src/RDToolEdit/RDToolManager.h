// =====================================================================================
// 
//       Filename:  RDToolManager.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/06/11 22:01:13
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDTOOLMANAGER_INC
#define  RDTOOLMANAGER_INC
#include <map>
#include <QString>
#include <QObject>

class RDBaseTool;
class RDDocument;
typedef std::map<QString,RDBaseTool*>::iterator RDToolIt;

class RDToolManager :public QObject
{
    Q_OBJECT
public:
    static RDToolManager* GetToolManager();
    static void ReleaseToolManager();
    ~RDToolManager();
    RDToolIt GetBeginTool(){return m_vecTool.begin();}
    RDToolIt GetEndTool(){return m_vecTool.end();}
    bool SwitchTool(const QString* newToolName,RDDocument& pDocument); 
    void StopCurTool();

    const RDBaseTool* GetCurTool()const{return m_pCurTool;}
    RDBaseTool* GetCurTool(){return m_pCurTool;}

    RDDocument* GetDocument(){return m_pCurDoc;}
    void SendSceneChange(){emit SceneChange();}
signals:
    void ChangeTool(const QString& toolName);
    void SceneChange();
protected:
    RDToolManager();
    void RegisterTool();
    void RegisterTool(RDBaseTool* pTool);
protected:
    std::map<QString,RDBaseTool*> m_vecTool;
    RDBaseTool* m_pCurTool;
    RDDocument* m_pCurDoc;
////////////////////////////////////////////////////////////////////////////////
    static RDToolManager* m_pToolManager;
};
#endif   // ----- #ifndef rdtoolmanager_INC  -----
