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

class QGraphicsScene;
class RDScene;
class QResizeEvent;
class RDNode;
class QUuid;
class RDSectionView :public QGraphicsView
{
public:
    RDSectionView(int nScale,RDScene* pScene,QWidget* pWidget );
	void SetSceneNode(RDScene* pScene);
protected:
    void AddChildNodeSection(int& nIndex,RDNode* pNode,const QUuid& idStory);
	//virtual void resizeEvent(QResizeEvent* event); 
protected:
	RDScene* m_pScene;
	int	 m_nScale;
};

#endif   // ----- #ifndef rdscetionview_INC  -----
