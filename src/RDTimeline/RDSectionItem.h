/*
 * =====================================================================================
 *
 *       Filename:  RDSectionItem.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/04/12 17:15:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <QGraphicsItem>
#include "mac_define.h"
#include <vector>
#include <QPainterPath>

using  namespace std;
class RDSection;
class RDNode;
class RDKeyItem;
class RDSectionItem : public QGraphicsItem
{
public:
    RDSectionItem(RDNode* pNode,RDSection* pSection,int nHeight,int nYOffset);
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	bool	isEqual(const RDNode* pNode)const{return m_pNode == pNode;}
	void	removeSection();
    void    itemChange();
protected:
    void createKeyItem();
    void removeKeyItem();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void SetSectionType();
    void DrawKey(RDTime time,float fScale,QPainter *painter);
protected:
    QPainterPath    m_shape;
    vector<RDTime> m_nHitKey;
    int         m_nHeight;
    int         m_nYOffset;
    RDNode*     m_pNode;
    RDSection*  m_pSection;
    QImage      m_imgSectionType;
    vector<RDKeyItem*> m_vecKeyItem;
};
