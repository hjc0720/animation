// =====================================================================================
// 
//       Filename:  RDDocument.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 13:56:33
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDDocument.h"
#include "RDProject.h"
#include <QFile>
#include "mac_define.h"
#include "RDSceneRenderData.h"
#include "RDScene.h"
#include <QDir>
#include <QMutexLocker>
#include "RDFileDataStream.h"
#include "RDResourceManager.h"
#include <QDebug>
#include "RDStory.h"
#include "RDLayer.h"
#include "RDCamera.h"
#include "RDLight.h"

// =====================================================================================
class RDUndoRedo :public QUndoCommand
{
public:
    RDUndoRedo(RDUndoCommand* cmd,RDDocument* pDoc);
    virtual ~RDUndoRedo(){SAFE_DELETE(m_cmd)}
    void undo();
    void redo();
protected:
    RDDocument* m_pDoc;
    RDUndoCommand* m_cmd;
};

inline RDUndoRedo::RDUndoRedo(RDUndoCommand* cmd,RDDocument* pDoc)
:m_pDoc(pDoc),m_cmd(cmd)
{
    setText(m_cmd->GetText());
}

inline void RDUndoRedo::undo()
{
    RDNode* pNode = m_pDoc->GetNode(m_cmd->GetNodeID());
    m_cmd->SetNode(pNode);
    qDebug() << m_cmd->GetText() << pNode->GetNodeID();
    m_cmd->undo();
}

inline void RDUndoRedo::redo()
{
    RDNode* pNode = m_pDoc->GetNode(m_cmd->GetNodeID());
    m_cmd->SetNode(pNode);
    m_cmd->redo();
}

// =====================================================================================
RDDocument::RDDocument(bool bCreateNewProj)
    :m_nEditType(RDEdit_AutoKey)
    ,m_nCurSceneIndex(-1)
     ,m_fScale(1)
     ,m_nCurFrame(0)
     ,m_DocUUID(QUuid::createUuid())
{
    if(bCreateNewProj)
    {
        m_pProject = new RDProject(1920,1080,50);
        SetCurScene(0);

        RDScene* pScene = GetCurScene();
        RDSceneRenderData* pRenderData = dynamic_cast<RDSceneRenderData* >(pScene->GetRenderData(DEFAULT_RD));
        pScene->TriggerStory(0,0,*pRenderData);
        auto pStory = pRenderData->GetCurTrigStory();
		pScene->AddSection(m_nCurFrame - pStory.GetStartTime(),1000000000,pStory.GetStoryId());

        RDLayer* pLayer = new RDLayer(RD3DLayer,"layer");
        pLayer->SetParent(pScene);
        pScene->AddChild(*pLayer);
        pLayer->AddSection(m_nCurFrame - pStory.GetStartTime(),1000000000,pStory.GetStoryId());

        for(size_t i = 0; i < pLayer->GetCameraCount(); i++)
        {
            pLayer->GetCamera(i)->AddSection(m_nCurFrame - pStory.GetStartTime(),1000000000,pStory.GetStoryId());
        }
        for(size_t i = 0; i < pLayer->GetLightCount(); i++)
        {
            pLayer->GetLight(i)->AddSection(m_nCurFrame - pStory.GetStartTime(),1000000000,pStory.GetStoryId());
        }

        PushTopNode(pLayer);

        CreateTempProjDir();
    }
#ifdef _DEBUG
    qDebug() << m_DocUUID.toString();
#endif
}
RDDocument::~RDDocument()
{
    CloseProj();
    DeleteTempProjDir();
}
void RDDocument::SaveProj()
{
    if(m_pProject)
        SaveProj(*m_pProject);
}
void RDDocument::SaveProj(RDProject& pProj)
{
    SaveProjAs(pProj,pProj.GetFilePath());
}
void RDDocument::SaveProjAs(const QString& filePath)
{
    if(m_pProject)
    {
        SaveProjAs(*m_pProject,filePath);
        m_pProject->SetFilePath(filePath);
    }
}

const char* MAGICNUM = "JENNYHJC";
void RDDocument::SaveProjAs(RDProject& pProj,const QString& filePath)
{
    QFile out(m_strCachePath + "/project");
	qDebug() << " begin save file" << filePath;
    out.open(QIODevice::WriteOnly);
    RDFileDataStream data(&out,m_strCachePath + "/Resource");
    data << QString(MAGICNUM);
    pProj.Serialize(data,true);
    data.EndSaveResource();
    out.close();

    TarProjDir(filePath);
	qDebug() << " end save file" << filePath;
}
void RDDocument::LoadProj(const QString& filePath)
{
    CloseProj();
    if(!m_strCachePath.isEmpty())
        DeleteTempProjDir();

    qDebug()<<filePath;
    QFile in(filePath);
    if(!in.open(QIODevice::ReadOnly))
        return;
    in.close();

    CreateTempProjDir();
    UntarProjDir(filePath);
    in.setFileName(m_strCachePath + "/project");
    qDebug()<<"project path:" <<m_strCachePath;
    if(!in.open(QIODevice::ReadOnly))
        return;
    RDFileDataStream data(&in);
    m_pProject = new RDProject();
    QString magicNum;
    data >> magicNum;
    qDebug() << magicNum;
    if(magicNum != QString(MAGICNUM))
    {
        in.close();
        return;
    }
    m_pProject->Serialize(data,false);
    m_pProject->SetFilePath(filePath);
    SetCurScene(0);
}
void RDDocument::CloseProj()
{
    SetCurScene(-1);
    SAFE_DELETE(m_pProject);
}
bool RDDocument::bHavePath()
{
    return !m_pProject->GetFilePath().isEmpty();
}
const QString& RDDocument::GetProjPath()
{
    return m_pProject->GetFilePath();
}
void RDDocument::SetProjPath(const QString& path)
{
    m_pProject->SetFilePath(path);
}
void RDDocument::SetCurScene(int nSceneIndex)
{
    if(m_nCurSceneIndex == nSceneIndex)
        return;
    RDScene* pScene = GetCurScene();
    if(pScene)
        pScene->RemoveRenderData();
    ClearRDStack();
    ClearSelItem();
    m_nCurSceneIndex = nSceneIndex;
    if(nSceneIndex >= 0)
    {
        pScene = GetCurScene();
        RDRenderData* pData = pScene->GetRenderData(DEFAULT_RD);
        pData->setChangeLevel(RDRender_GraphicChange);
        pScene->SetWidthHeight(m_pProject->GetWidth(),m_pProject->GetHeight());
        PushTopNode(pScene);
        pScene->setRenderScale(m_fScale,DEFAULT_RD);
    }
}
void RDDocument::CreateTempProjDir()
{
    m_strCachePath = QDir::homePath();
    m_strCachePath += "/.Animation/cache/";
    m_strCachePath += m_DocUUID.toString();
    QString cmdStr("mkdir ");
    cmdStr += m_strCachePath;
    system(cmdStr.toAscii().data());

    QString cmdStr2("mkdir ");
    cmdStr2 += m_strCachePath;
    cmdStr2 += "/Resource";
    system(cmdStr2.toAscii().data());

//image
    QString imageStr2("mkdir ");
    imageStr2 += m_strCachePath;
    imageStr2 += "/Resource/Image";
    system(imageStr2.toAscii().data());
//movie
    QString movieStr2("mkdir ");
    movieStr2 += m_strCachePath;
    movieStr2 += "/Resource/Movie";
    system(movieStr2.toAscii().data());

#ifdef _DEBUG
    qDebug() << m_strCachePath;
#endif
}

int RDDocument::DeleteTempProjDir()
{
    QString cmdStr("rm -rf ");
    cmdStr += m_strCachePath;
    int nRet = system(cmdStr.toAscii().data());
    if(nRet)
        m_strCachePath.clear();
    return nRet;
}
int RDDocument::TarProjDir(const QString& strProjPath)
{
    QString cmdStr1("cd ");
    cmdStr1 += m_strCachePath;
    cmdStr1 += " && ";
    QString cmdStr2("tar -jcf ");
    cmdStr2 += strProjPath;
    cmdStr2 += " ";
    cmdStr2 += "*";

    cmdStr1 += cmdStr2;

    int nRet = system(cmdStr1.toAscii().data());
    //system(cmdStr2.toAscii().data());
#ifdef _DEBUG
    qDebug() << cmdStr1;
    qDebug() << cmdStr2;
#endif
    return nRet;
}

void RDDocument::UntarProjDir(const QString& strProjPath)
{
    QString cmdStr("tar -jxf ");
    cmdStr += strProjPath;
    cmdStr += " -C ";
    cmdStr += m_strCachePath;

    qDebug()  << cmdStr;
    system(cmdStr.toAscii().data());

    QString copyStr("cp -rf ");
    copyStr += m_strCachePath + "/Resource/* ";
    copyStr += RDResourceManager::GetResourceManager()->GetResourcePath();

    qDebug()  << copyStr;
    system(copyStr.toAscii().data());
}
void RDDocument::ClearRDStack()
{
    while (!m_EditRDStack.empty())
        m_EditRDStack.pop();
}
void RDDocument::AddChildNode(RDNode& parent,RDNode& pChild)
{
    RDRenderData* pRenderData = parent.GetRenderData(DEFAULT_RD);
    auto pStory = pRenderData->GetCurTrigStory();
    pChild.AddSection(m_nCurFrame - pStory.GetStartTime(),1000000000,pStory.GetStoryId());

    parent.Lock();
    parent.AddChild(pChild);
    parent.UnLock();

    ClearSelItem();
    AddSelItem(pChild);
}

void RDDocument::DelSelItems()
{
    qDebug() << "del select Items:" <<m_SelItemList.size();
    for(size_t i = 0; i < m_SelItemList.size(); i++)
    {
        RDNode* pNode = m_SelItemList[i];
        RDNode* pParentNode = pNode->GetParent();
        pParentNode->SetChangeLevel(RDRender_GraphicChange);
        pParentNode->RemoveChild(*pNode);
        qDebug() << pNode->GetName();
        SAFE_DELETE(pNode);
    }
    ClearSelItem();
}
void RDDocument::SetScale(float fScale)
{
    m_fScale = fScale;
    GetCurScene()->setRenderScale(m_fScale,DEFAULT_RD);
}
void RDDocument::AddUndoCommand(RDUndoCommand* cmd)
{
    m_UndoStack.push(new RDUndoRedo(cmd,this));
}
void RDDocument::UndoBegin(const QString& text)
{
	m_UndoStack.beginMacro(text);
}
void RDDocument::UndoEnd()
{
	m_UndoStack.endMacro();
}

RDNode* RDDocument::GetNode(const QUuid& NodeId)
{
    size_t nCount = m_pProject->GetSceneCount();
    for(size_t i = 0; i != nCount; i++)
    {
        RDScene* pScene = m_pProject->GetScene(i);
        if(pScene->GetNodeID() == NodeId)
            return pScene;
        auto ret = pScene->GetChild(NodeId);
        if(ret)
            return ret;
    }
    return 0;
}

void RDDocument::AddStoryAndTrigger(const std::string& StoryName)
{
	size_t nIndex = GetCurScene()->AddStory(StoryName);
	TriggerStory(nIndex);
}

bool RDDocument::RemoveStoryAndTrigger(size_t nIndex)
{
	if(!GetCurScene()->RemoveStory(nIndex))
		return false;

	nIndex = std::min(GetCurScene()->GetStoryCount() - 1,nIndex);
	TriggerStory(nIndex);
	return true;
}

void RDDocument::TriggerStory(size_t nIndex)
{
	RDSceneRenderData* pRenderData = dynamic_cast<RDSceneRenderData* >(GetCurScene()->GetRenderData(DEFAULT_RD));
	RDTime CurStoryStartTime = pRenderData->GetCurTrigStory().GetStartTime();
	GetCurScene()->TriggerStory(nIndex,CurStoryStartTime + m_nCurFrame,*pRenderData);
}
