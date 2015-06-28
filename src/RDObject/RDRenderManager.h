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
#include <string>
#include <array>
#include <map>
#include <memory>
#include <functional>
class RDScene;
class QPointF;
class RDBuffer;
class QRectF;
class RDNode;

enum RDRenderCallbackType
{
    RDRender_LayerCalFrameEnd,
    RDRender_SceneCalFrameEnd,
    RDRender_LayerRenderBegin,//enable depth
    RDRender_SceneRenderEnd,
    RDRender_CallbackCount
};

class RenderManager;
typedef std::function<bool(RDNode*,RenderManager*)> RenderManagerCallback;

class RenderManager
{
public:
    void SetRenderName(const std::string& pName){m_RenderName = pName;}
    void SetScene(RDScene* pScene){m_pScene = pScene;}
    bool RenderScene(const QPointF& offSceneToScreen,const QRectF& rtScene,RDTime nTime);
    void RegistCallback(RDRenderCallbackType eType,RenderManagerCallback& pCallback);
    void Call(RDRenderCallbackType eType,RDNode* pLayer);
    const std::string& getRenderName(){return m_RenderName;}
protected:
    RDScene*      m_pScene;
    std::string       m_RenderName;
    std::map<RDRenderCallbackType,RenderManagerCallback>        m_Callback;
};
#endif   // ----- #ifndef rdrendermanager_INC  -----
