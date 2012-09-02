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
     :m_nProjVersion(g_nNowProjVersion)
{

}
RDProject::RDProject(int nWidth,int nHeight,double dFrameRate)
        :m_nProjVersion(g_nNowProjVersion)
        ,m_nWidth(nWidth)
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

RDFileDataStream& operator << (RDFileDataStream& buffer,const RDProject& proj)
{
    buffer << proj.m_nProjVersion;
    buffer << proj.m_nWidth;
    buffer << proj.m_nHeight;
    buffer << proj.m_dFrameRate;
    buffer << (quint64)proj.m_SceneList.size();
    for(size_t i = 0; i < proj.m_SceneList.size();i++)
    {
        buffer << *proj.m_SceneList[i];
    }
    return buffer;
}
RDFileDataStream& operator >> (RDFileDataStream& buffer,RDProject& proj)
{
    buffer >> proj.m_nProjVersion;
    buffer >> proj.m_nWidth;
    buffer >> proj.m_nHeight;
    buffer >> proj.m_dFrameRate;
    quint64 nSceneCount = 0;
    buffer >> nSceneCount;
    for(quint64 i = 0; i < nSceneCount; i++)
    {
        RDScene* pScene = new RDScene;
        buffer >> *pScene;
        proj.m_SceneList.push_back(pScene);
    }
    proj.m_nProjVersion = g_nNowProjVersion;
    return buffer;
}
