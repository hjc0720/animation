// =====================================================================================
// 
//       Filename:  RDProject.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  26/03/11 21:28:07
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDPROJECT_INC
#define  RDPROJECT_INC
#include <QString>
#include <stdexcept>
#include <vector>

class RDScene;
class RDFileDataStream;
class RDProject
{
public:
    RDProject();
    RDProject(int nWidth,int nHeight,double dFrameRate);
    ~RDProject();
    const QString& GetFilePath()const{return m_strFilePath;}
    void SetFilePath(const QString& newFilePath){m_strFilePath = newFilePath;}
    void CreateNewScene(const QString& strSceneName);
    int GetWidth()const{return m_nWidth;}
    int GetHeight()const{return m_nHeight;}
    RDScene* GetScene(int nIndex){try{return m_SceneList.at(nIndex);}catch(std::out_of_range& ){return 0;}}
    size_t GetSceneCount(){return m_SceneList.size();}

    void Serialize(RDFileDataStream& buffer,bool bSave);
protected:
    int     m_nWidth;
    int     m_nHeight;
    double  m_dFrameRate; //framecount per second;
    QString m_strFilePath;
    std::vector<RDScene*> m_SceneList;

};
#endif   // ----- #ifndef rdproject_INC  -----
