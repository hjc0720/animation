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
class RDObjHead;

class RDTimelineView :public QDockWidget
{
	Q_OBJECT
public:
		RDTimelineView(RDScene& pScene,QWidget* pParent);
signals:
        void    FrameChanged(const RDTime& );
        void    SectionChanged();
protected:
		void RDFillHead(RDNode& pNode);
		int GetHeadIndex(const RDNode& pNode);
        void UpdateBackground(int nStartIndex);
        void InsertObj(RDNode& pNewNode,int nIndex);
protected slots:
		void InsertObj(RDNode& pNewNode);
		void DelObj(RDNode& pNewNode);
        void collapseNode(RDNode& pNode);
protected:
		RDScene* m_pScene;
		QScrollArea* m_pHead;
        RDSectionView* m_pSectionView;
		QVBoxLayout* m_pHeadLayout;
        std::vector<RDObjHead*> m_objHead;
};
#endif   // ----- #ifndef rdtimeline_INC  -----
