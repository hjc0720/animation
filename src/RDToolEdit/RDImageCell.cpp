/*
 * =====================================================================================
 *
 *       Filename:  RDImageCell.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年04月25日 14时50分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Huang JianChao (hjc), hjc0720@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "RDImageCell.h"
#include "RDMd5.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QFileDialog>

RDImageCell* RDImageCell::GetImageCell()
{
    static RDImageCell* image = new RDImageCell(nullptr);
    return image;
}

RDImageCell::RDImageCell(QWidget* parent)
    :RDCell(tr("image"),parent)
{
    QHBoxLayout* pHLayout = new QHBoxLayout();

    QLabel* pImageFile = new QLabel(tr("Image File"));
    pHLayout->addWidget(pImageFile);

    m_pImageFile = new QLineEdit();
    m_pImageFile->setReadOnly(true);
    pHLayout->addWidget(m_pImageFile);

    QPushButton* pFileButton = new QPushButton("...");
    pFileButton->setMinimumWidth(25);
    pFileButton->setMaximumWidth(25);
    connect(pFileButton, SIGNAL(clicked()), this, SLOT(GetImageFile()));
    pHLayout->addWidget(pFileButton);

    AddLayout(pHLayout);

    m_pResolution = new QLabel();
    AddWidget(m_pResolution);

    pHLayout = new QHBoxLayout();
    QLabel* pWidth = new QLabel(tr("Width"));
    m_pWidth = new QSpinBox();
    connect(m_pWidth, SIGNAL(editingFinished()), this, SLOT(CellChange()));
    m_pWidth->setMinimum(1);
    QLabel* pHeight = new QLabel(tr("Height"));
    m_pHeight = new QSpinBox();
    m_pHeight->setMinimum(1);
    connect(m_pHeight, SIGNAL(editingFinished()), this, SLOT(CellChange()));
    pHLayout->addWidget(pWidth);
    pHLayout->addWidget(m_pWidth);
    pHLayout->addStretch();
    pHLayout->addWidget(pHeight);
    pHLayout->addWidget(m_pHeight);
    pHLayout->addStretch();
    AddLayout(pHLayout);
    
    pHLayout = new QHBoxLayout();
    QPushButton* pKeepSize = new QPushButton(QIcon(":/img_origin"),"");
    connect(pKeepSize, SIGNAL(clicked()), this, SLOT(OnOriginSize()));
    pHLayout->addWidget(pKeepSize);
    QPushButton* pKeepWidth = new QPushButton(QIcon(":/img_width"),"");
    connect(pKeepWidth, SIGNAL(clicked()), this, SLOT(OnKeepWidth()));
    pHLayout->addWidget(pKeepWidth);
    QPushButton* pKeepHeight = new QPushButton(QIcon(":/img_height"),"");
    connect(pKeepHeight, SIGNAL(clicked()), this, SLOT(OnKeepHeight()));
    pHLayout->addWidget(pKeepHeight);
    pHLayout->addStretch();
    AddLayout(pHLayout);
}

const RDMd5& RDImageCell::GetCellMd5()
{
    const static RDMd5 ImageCellMd5("RDImageCell",-1);
    return ImageCellMd5;
}

void RDImageCell::GetImageFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Get Picture"));
    if(fileName == "")
        return;
    SetImageFileName(fileName);
    emit CellChanged(GetCellMd5(),RDImageFileChange);
}

void    RDImageCell::SetImageFileName(const QString& fileName)
{
    QFileInfo file(fileName);
    m_pImageFile->setText(file.fileName());
    m_pImageFile->setToolTip(fileName);
}

QString RDImageCell::GetImageFileName()const
{
    return m_pImageFile->toolTip();
}

void    RDImageCell::SetImageOriginSize(int nWidth,int nHeight)
{
    m_pWidth->setMaximum(nWidth);
    m_pHeight->setMaximum(nHeight);
    static QString strResolution(tr("Resolution:\t"));
    m_pResolution->setText(strResolution + QString::number(nWidth) + " * " + QString::number(nHeight));
}

void    RDImageCell::SetImageSize(int nWidth,int nHeight)
{
    m_pWidth->setValue(nWidth);
    m_pHeight->setValue(nHeight);
}

int     RDImageCell::GetImageWidth()const
{
    return m_pWidth->value();
}

int     RDImageCell::GetImageHeight()const
{
    return m_pHeight->value();
}

void RDImageCell::OnKeepWidth()
{
    m_pHeight->setValue(m_pHeight->maximum() * m_pWidth->value() / m_pWidth->maximum());
    emit CellChanged(GetCellMd5(),RDImageSizeChange);
}

void RDImageCell::OnKeepHeight()
{
    m_pWidth->setValue(m_pWidth->maximum() * m_pHeight->value() / m_pHeight->maximum());
    emit CellChanged(GetCellMd5(),RDImageSizeChange);
}

void RDImageCell::OnOriginSize()
{
    m_pWidth->setValue(m_pWidth->maximum());
    m_pHeight->setValue(m_pHeight->maximum());
    emit CellChanged(GetCellMd5(),RDImageSizeChange);
}
