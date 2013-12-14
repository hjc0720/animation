// =====================================================================================
// 
//       Filename:  RDProject.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 21:27:35
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDProject.h"
#include "RDScene.h"
#include "mac_define.h"
#include "RDFileDataStream.h"

const int g_nNowProjVersion = 0;

RDProject::RDProject()
{

}
RDProject::RDProject(int nWidth,int nHeight,double dFrameRate)
        :m_nWidth(nWidth)
        ,m_nHeight(nHeight)
        ,m_dFrameRate(dFrameRate)
{
    CreateNewScene("scene");
}
RDProject::~RDProject()
{
    for(size_t i = 0; i < m_SceneList.size(); i++)
    {
        RDScene* pScene = m_SceneList[i];
        SAFE_DELETE(pScene);
    }
}

void RDProject::CreateNewScene(const QString& strSceneName)
{
   RDScene* pscene = new RDScene(strSceneName);
   m_SceneList.push_back(pscene);
}

void RDProject::Serialize(RDFileDataStream& buffer,bool bSave)
{
    int nVersion = g_nNowProjVersion;
    buffer.Serialize(nVersion,bSave);
    buffer.Serialize(m_nWidth,bSave);
    buffer.Serialize(m_nHeight,bSave);
    buffer.Serialize(m_dFrameRate,bSave);
    int nSceneListSize = static_cast<int>(m_SceneList.size());
    buffer.Serialize(nSceneListSize,bSave);
    for(int i = 0; i < nSceneListSize;i++)
    {
        RDScene* pScene = nullptr;
        if(bSave)
            pScene = m_SceneList[i];
        else
        {
            pScene = new RDScene;
            m_SceneList.push_back(pScene);
        }
        pScene->Serialize(buffer,bSave);
    }
}
