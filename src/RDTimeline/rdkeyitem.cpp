/*
 * =====================================================================================
 *
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  HuanJianChao (), hjc0720@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "rdkeyitem.h"
#include <QPainter>
#include <QPointF>
#include "RDSection.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QKeyEvent>
#include <QGraphicsScene>
#include "RDNode.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include "rddoublespinbox.h"


const float RDKeyItem::value = 6 ;
RDKeyItem::RDKeyItem(int nHeight, RDTime time, const std::list<RDSingleKey> &list, QGraphicsItem* parent)
    :QGraphicsObject(parent)
    ,m_nHeight(nHeight)
    ,m_nTime(time)
    ,m_keylist(list)
{
    setFlags(ItemIsMovable | ItemIsFocusable);
}

QRectF RDKeyItem::boundingRect() const
{
    return QRectF(-m_fRealWidth,value,2*m_fRealWidth,m_nHeight-2*value);
}

void RDKeyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    qreal fScale = painter->worldTransform().m11();
    m_fRealWidth = (m_nHeight - value * 2) / 2 / fScale;
    QPointF pt[4];

    pt[0].setX(0);
    pt[0].setY(value);

    pt[1].setX(- m_fRealWidth);
    pt[1].setY(m_nHeight / 2.f );

    pt[2].setX(0);
    pt[2].setY(m_nHeight - value);

    pt[3].setX(m_fRealWidth);
    pt[3].setY(m_nHeight / 2.f);


    if(isSelected())
    {
        painter->setBrush(Qt::yellow);
    }
    else
    {
        painter->setPen(Qt::green);
    }
    painter->drawPolygon(pt,4);

    //qDebug() << pt[0] << pt[1] << pt[2] << pt[3];
}


void RDKeyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    RDTime offsetPos = event->scenePos().x() - event->lastScenePos().x();
    emit TimeMoved(m_nTime,m_nTime+offsetPos);
    m_nTime+=offsetPos;
    setPos(m_nTime,0);
}

Q_DECLARE_METATYPE(float3)

void RDKeyItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    QDialog* pEditDlg = new QDialog();
    QFormLayout* pLayout = new QFormLayout(pEditDlg);
    pEditDlg->setLayout(pLayout);

    std::vector<QVariant> initValue;
    for(auto it = m_keylist.begin(); it != m_keylist.end(); it++)
    {
        if(it->key->ValueType() == RDFloat3)
        {
            RDKey<float3>* pKey = dynamic_cast<RDKey<float3>*>(it->key);
            addKeyEdit(pLayout,it->type,pKey);
            QVariant v;
            v.setValue(pKey->GetValue());
            initValue.push_back(v);
        }
    }

    QDialogButtonBox* pButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, pEditDlg);
    connect(pButton,SIGNAL(accepted()),pEditDlg,SLOT(accept()));
    connect(pButton,SIGNAL(rejected()),pEditDlg,SLOT(reject()));
    pLayout->addRow(nullptr,pButton);

    if(!pEditDlg->exec())
    {
        restoreValue(initValue);
        emit valueChanged(m_nTime);
    }
}

void RDKeyItem::restoreValue(const std::vector<QVariant> &initValue)
{
    auto value = initValue.begin();
    for(auto it = m_keylist.begin(); it != m_keylist.end(); it++)
    {
        if(it->key->ValueType() == RDFloat3)
        {
            RDKey<float3>* pKey = dynamic_cast<RDKey<float3>*>(it->key);
            pKey->SetValue(value->value<float3>());
            value++;
        }
    }
}

void RDKeyItem::addKeyEdit(QFormLayout *pLayout, const std::string &type, RDKey<float3> *pKey)
{
    pLayout->addRow(type.c_str(),(QWidget*)nullptr);

    RDDoubleSpinBox* x = new RDDoubleSpinBox(true);
    x->updateValue(pKey->GetValue().x());
    connect(x,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double v){
        float3 value = pKey->GetValue();
        value.SetX(v);
        pKey->SetValue(value);
        emit valueChanged(m_nTime);
    });
    pLayout->addRow("x",x);

    RDDoubleSpinBox* y = new RDDoubleSpinBox(true);
    y->updateValue(pKey->GetValue().y());
    connect(y,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double v){
        float3 value = pKey->GetValue();
        value.SetY(v);
        pKey->SetValue(value);
        emit valueChanged(m_nTime);
    });
    pLayout->addRow("y",y);

    RDDoubleSpinBox* z = new RDDoubleSpinBox(true);
    z->updateValue(pKey->GetValue().z());
    connect(z,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double v){
        float3 value = pKey->GetValue();
        value.SetZ(v);
        pKey->SetValue(value);
        emit valueChanged(m_nTime);
    });
    pLayout->addRow("z",z);
}
