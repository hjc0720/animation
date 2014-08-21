// =====================================================================================
// 
//       Filename:  RDRenderManager.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  12/04/11 21:02:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDRENDERMANAGER_INC
#define  RDRENDERMANAGER_INC
#include <QString>
#include "mac_define.h"

class RDScene;
class QPointF;
class RDBuffer;
class QRectF;

class RenderManager
{
public:
    void SetRenderName(const QString& pName){m_RenderName = pName;}
    void SetScene(RDScene* pScene){m_pScene = pScene;}
    bool RenderScene(const QPointF& offSceneToScreen,const QRectF& rtScene,RDTime nTime);
protected:
    RDScene*      m_pScene;
    QString       m_RenderName;
};
#endif   // ----- #ifndef rdrendermanager_INC  -----
