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

class RDScene;
class QPointF;
class RDBuffer;
class QRectF;

class RenderManager
{
public:
    void SetRenderName(const QString& pName){m_RenderName = pName;}
    void SetScene(RDScene* pScene){m_pScene = pScene;}
    void SetDstBuffer(RDBuffer* pDstBuffer){m_pDstBuffer = pDstBuffer;}
    bool RenderScene(const QPointF& offSceneToScreen,const QRectF& rtScene,unsigned long nTime);
protected:
    RDScene*      m_pScene;
    RDBuffer*     m_pDstBuffer;
    QString       m_RenderName;
};
#endif   // ----- #ifndef rdrendermanager_INC  -----
