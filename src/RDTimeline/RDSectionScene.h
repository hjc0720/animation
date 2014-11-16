/*
 * =====================================================================================
 *
 *       Filename:  RDSectionScene.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年11月16日 22时02分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huang jian chao (), huangjianchao@xiaomi.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  RDSECTIONSCENE_INC
#define  RDSECTIONSCENE_INC

#include <QGraphicsScene>
#include "mac_define.h"

class RDNode;
class RDTimeMarker;

class RDSectionScene : public QGraphicsScene
{
	Q_OBJECT
public:
		RDSectionScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
		void ChangeFrame(const RDTime& nFrame);
	
		void SetTimeMarker(RDTimeMarker* pMarker ){m_pTimeMarker = pMarker;}
		RDTimeMarker* GetTimeMarker(){return m_pTimeMarker;}
		void    SectionChange(){emit SectionChanged();};
		void 	delNode(const RDNode* pNode);
signals:
		void    FrameChanged(const RDTime& nFrame);
		void    SectionChanged();
protected:
		RDTimeMarker* m_pTimeMarker;
};
#endif   /* ----- #ifndef RDSectionScene_INC  ----- */
