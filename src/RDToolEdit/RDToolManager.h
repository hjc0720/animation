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
class QCursor;
typedef std::map<std::string,RDBaseTool*>::iterator RDToolIt;

class RDToolManager :public QObject
{
    Q_OBJECT
public:
    static RDToolManager* GetToolManager();
    static void ReleaseToolManager();
    ~RDToolManager();
    RDToolIt GetBeginTool(){return m_vecTool.begin();}
    RDToolIt GetEndTool(){return m_vecTool.end();}
    bool SwitchTool(const std::string &newToolName, RDDocument& pDocument);
    void StopCurTool();

    const RDBaseTool* GetCurTool()const{return m_pCurTool;}
    RDBaseTool* GetCurTool(){return m_pCurTool;}

    RDDocument* GetDocument(){return m_pCurDoc;}
    void SendSceneChange(){emit SceneChange();}
	void sendDelSelItems(){emit deleteSelItems();}
	void 	CursorChange(QCursor& cursor){emit cursorChange(cursor);}
signals:
    void ChangeTool(const std::string& toolName);
    void SceneChange();
	void deleteSelItems();
	void 	cursorChange(QCursor&);
protected:
    RDToolManager();
    void RegisterTool();
    void RegisterTool(RDBaseTool* pTool);
protected:
    std::map<std::string,RDBaseTool*> m_vecTool;
    RDBaseTool* m_pCurTool;
    RDDocument* m_pCurDoc;
////////////////////////////////////////////////////////////////////////////////
    static RDToolManager* m_pToolManager;
};
#endif   // ----- #ifndef rdtoolmanager_INC  -----
