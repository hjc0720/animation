/*
 * =====================================================================================
 *
 *       Filename:  RDSectionScene.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年11月16日 22时02分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huang jian chao (), huangjianchao@xiaomi.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RDSectionScene.h"
#include "RDTimeMarker.h"
#include "RDSectionItem.h"

RDSectionScene::RDSectionScene( qreal x, qreal y, qreal width, qreal height, QObject * parent /* = 0  */ )
			:QGraphicsScene(x,y,width,height,parent)
			 ,m_pTimeMarker(nullptr)
{
}

void RDSectionScene::ChangeFrame(const RDTime& nFrame)
{
	if(m_pTimeMarker)
		m_pTimeMarker->SetTime(nFrame);
	
	emit FrameChanged(nFrame);
}

void 	RDSectionScene::delNode(const RDNode* pNode)
{
	QList<QGraphicsItem *> sectionList = items();
    std::vector<QGraphicsItem *> removeSection;
	for(int i = 0; i < sectionList.size(); i++)
	{
		RDSectionItem* pItem = dynamic_cast<RDSectionItem*>(sectionList[i]);
		if(pItem && pItem->isEqual(pNode))
		{
            removeSection.push_back(pItem);
		}
    }

    for(QGraphicsItem* item:removeSection)
    {
        removeItem(item);
        SAFE_DELETE(item);
    }
}

void RDSectionScene::itemChange(const RDNode *pNode)
{
    for(QGraphicsItem* item:items())
    {
        RDSectionItem* pItem = dynamic_cast<RDSectionItem*>(item);
        if(pItem && pItem->isEqual(pNode))
        {
            pItem->itemChange();
        }
    }
}
