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
#include "mac_define.h"
#include "RDTimeMarker.h"

class RDScene;
class QResizeEvent;
class RDNode;
class QUuid;
class RDStory;

class RDSectionView :public QGraphicsView
{
    Q_OBJECT
public:
    RDSectionView(RDScene* pScene,const RDStory* pStory,QWidget* pWidget );
	void SetSceneNode(RDScene* pScene);
    void SetScale(const RDTime& nScale); 
	void DelNode(RDNode* pNode);
signals:
    void    FrameChanged(const RDTime& nFrame);
    void    SectionChanged();
protected:
    void AddChildNodeSection(int& nIndex,RDNode* pNode,const QUuid& idStory);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void wheelEvent(QWheelEvent *event);
protected:
	RDScene* m_pScene;
    const RDStory* m_pStory;
	int	 m_nScale;
};

#endif   // ----- #ifndef rdscetionview_INC  -----
