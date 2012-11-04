// =====================================================================================
// 
//       Filename:  RDTimeline.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  30/11/11 21:22:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDTIMELINE_INC
#define  RDTIMELINE_INC
#include <QDockWidget>
#include "mac_define.h"

class RDScene;
class RDNode;
class QVBoxLayout;
class QScrollArea;
class RDTimeRuler;
class RDSectionView;

class RDTimelineView :public QDockWidget
{
	Q_OBJECT
public:
		RDTimelineView(RDScene& pScene,QWidget* pParent);
signals:
        void    FrameChanged(const RDTime& );
protected:
		void RDFillHead(RDNode& pNode);
		int GetHeadIndex(const RDNode& pNode);
protected slots:
		void InsertObj(RDNode& pNewNode);
		void DelObj(RDNode& pNewNode);
protected:
		RDScene* m_pScene;
		QScrollArea* m_pHead;
        RDSectionView* m_pSectionView;
		QVBoxLayout* m_pHeadLayout;
		RDTimeRuler* m_pRuler;
};
#endif   // ----- #ifndef rdtimeline_INC  -----