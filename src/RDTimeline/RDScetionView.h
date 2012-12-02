// =====================================================================================
// 
//       Filename:  RDScetionView.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/11/2012 05:52:44 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDSCETIONVIEW_INC
#define  RDSCETIONVIEW_INC
#include <QGraphicsView>
#include <QGraphicsScene>
#include "mac_define.h"
#include "RDTimeMarker.h"

class RDScene;
class QResizeEvent;
class RDNode;
class QUuid;

class RDSectionScene : public QGraphicsScene
{
    Q_OBJECT
public:
        RDSectionScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 )
            :QGraphicsScene(x,y,width,height,parent)
             ,m_pTimeMarker(nullptr)
    { }
        void ChangeFrame(const RDTime& nFrame)
        {
            if(m_pTimeMarker)
            {
                m_pTimeMarker->SetTime(nFrame);
            }
            emit FrameChanged(nFrame);
        }
        void SetTimeMarker(RDTimeMarker* pMarker ){m_pTimeMarker = pMarker;}
        RDTimeMarker* GetTimeMarker(){return m_pTimeMarker;}
signals:
        void    FrameChanged(const RDTime& nFrame);
protected:
        RDTimeMarker* m_pTimeMarker;
};

class RDSectionView :public QGraphicsView
{
    Q_OBJECT
public:
    RDSectionView(RDScene* pScene,QWidget* pWidget );
	void SetSceneNode(RDScene* pScene);
    void SetScale(const RDTime& nScale); 
signals:
    void    FrameChanged(const RDTime& nFrame);
protected:
    void AddChildNodeSection(int& nIndex,RDNode* pNode,const QUuid& idStory);
    virtual void resizeEvent(QResizeEvent* event); 
protected:
	RDScene* m_pScene;
	int	 m_nScale;
};

#endif   // ----- #ifndef rdscetionview_INC  -----
