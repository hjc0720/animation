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
#include <iterator>

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

void RDProject::CreateNewScene(const std::string& strSceneName)
{
   RDScene* pscene = new RDScene(strSceneName);
   m_SceneList.push_back(pscene);
}

void RDProject::Serialize(RDJsonDataStream& buffer,Json::Value& parent,bool bSave)
{
    buffer.Serialize(parent,"width",bSave,m_nWidth);
    buffer.Serialize(parent,"height",bSave,m_nHeight);
    buffer.Serialize(parent,"frame_rate",bSave,m_dFrameRate);
    buffer.Serialize(parent,"scenes",bSave,m_SceneList.begin(),m_SceneList.end(),std::back_inserter(m_SceneList),[&buffer](Json::Value& child,RDScene* scene,bool bSave)->RDScene*{
        if(!bSave)
            scene = new RDScene;
        scene->Serialize(buffer,child,bSave);
        return scene;
    });
}
