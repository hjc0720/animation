// =====================================================================================
// 
//       Filename:  RDToolManager.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  29/06/11 22:01:09
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDToolManager.h"
#include "mac_define.h"
#include "RDImageTool.h"
#include "RDSelectTool.h"
#include "RDDocument.h"
#include <QDebug>

const std::string g_strDefaultTool("select tool");
RDToolManager* RDToolManager::m_pToolManager = NULL;

RDToolManager* RDToolManager::GetToolManager()
{
    if(!m_pToolManager)
        m_pToolManager = new RDToolManager();
    return m_pToolManager;
}
void RDToolManager::ReleaseToolManager()
{
    SAFE_DELETE(m_pToolManager);
}

////////////////////////////////////////////////////////////////////////////////
RDToolManager::RDToolManager()
    :m_pCurTool(NULL)
{
    RegisterTool();
    m_pCurDoc = NULL;
}

RDToolManager::~RDToolManager()
{
}

void RDToolManager::RegisterTool()
{
    RegisterTool(new RDSelectTool);
    RegisterTool(new RDImageTool);
}

void RDToolManager::RegisterTool(RDBaseTool* pTool)
{
    m_vecTool[pTool->GetName()] = pTool;
}

bool RDToolManager::SwitchTool(const std::string& pNewToolName,RDDocument& pDocument)
{
    if(!m_pCurTool && pNewToolName.empty())
        return false; 
    m_pCurDoc = &pDocument;
    if(m_pCurTool)
        m_pCurTool->EndEdit();
    if(!pNewToolName.empty())
        m_pCurTool = m_vecTool[pNewToolName];
    return m_pCurTool->BeginEdit(pDocument.GetTopNode());
} 

void RDToolManager::StopCurTool()
{
    SwitchTool(g_strDefaultTool,*m_pCurDoc);
    emit ChangeTool(g_strDefaultTool);
    emit SceneChange();
    qDebug() << "stop cur tool! emit signal";
}
