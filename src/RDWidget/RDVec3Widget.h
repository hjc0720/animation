/*
 * =====================================================================================
 *
 *       Filename:  RDVec3Widget.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年03月22日 10时32分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  RDVEC3WIDGET_INC
#define  RDVEC3WIDGET_INC
#include <QWidget>
#include "HVector3f.h"

class QDoubleSpinBox;
class RDVec3Widget :public QWidget
{
    Q_OBJECT
public:
    RDVec3Widget(QWidget* parent,bool bHorizon = true);
    ~RDVec3Widget();
    void SetMin(double dMin);
    void SetMax(double dMax);
    void SetRange(double dMin,double dMax);
    void SetStep(double val);
    void SetDecimals(int prec);

    void SetValue(const float3& value);
    const float3& value(){return m_vValue;}
signals:
    void Changed(const float3& value);
protected slots:
    void VectorChanged();
protected:
    float3 m_vValue;
    QDoubleSpinBox* m_pVector[3];
};
#endif   /* ----- #ifndef RDVec3Widget_INC  ----- */
