// =====================================================================================
// 
//       Filename:  RDSetSceneDlg.cpp:
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/05/11 21:59:38
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include "RDSetSceneDlg.h"
#include <QLabel>
#include <QAction>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include "RDColorWidget.h"
#include "RDImageResource.h"
#include "RDResourceManager.h"
#include <QFileDialog>

//dword to qcolor function, now I don't know where to put this function 
QColor GetColor(unsigned long color)
{
    int a = (color & 0xff000000) >> 24;
    int r = (color & 0x00ff0000) >> 16;
    int g = (color & 0x0000ff00) >> 8;
    int b = (color & 0x000000ff) ;
    return QColor(r,g,b,a);
}

unsigned long GetColor(const QColor& color)
{
    int a,r,g,b;
    color.getRgb(&r,&g,&b,&a); 
    return (((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff)));
}

RDSetSceneDlg::RDSetSceneDlg(QWidget * parent )
    :QDialog(parent)
{
    m_data.m_nBackType = RDScene_Back_Color;
    m_data.backColor = 0x00000000;
    //m_data.pImage = NULL;
    setWindowTitle(tr("Set Scene"));
    QHBoxLayout *horizontalLayout_2;
    QDialogButtonBox *buttonBox;
 
    backColor = new QRadioButton(tr("Scene Color"),this);
    backColor->setChecked(true);
    QColor initColor(0,0,0);
    Color = new RDColorWidget(initColor,this);
    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(backColor);
    horizontalLayout->addWidget(Color);
    horizontalLayout->addItem(horizontalSpacer);

    backImag = new QRadioButton(tr("Scene Image"),this);
    filePath = new QLineEdit(this);
    filePath->setMinimumSize(QSize(200, 0));
    getFile = new QPushButton(tr("..."),this);
    checkBox = new QCheckBox(tr("Strech"),this);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->addWidget(backImag);
    horizontalLayout_2->addWidget(filePath);
    horizontalLayout_2->addWidget(getFile);
    horizontalLayout_2->addWidget(checkBox);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);
    gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);
    gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(getFile, SIGNAL(clicked()), this, SLOT(GetImageFile()));
}

RDSetSceneDlg::~RDSetSceneDlg()
{
    ReleaseResource();
}

void RDSetSceneDlg::ReleaseResource()
{
    //if(m_data.m_nBackType == RDScene_Back_Picture && m_data.pImage)
    if(m_data.m_nBackType == RDScene_Back_Picture )
    {
        RDResourceManager* pManager = RDResourceManager::GetResourceManager();
        pManager->RemoveResource( *m_data.pImage);
        //m_data.pImage = NULL;
    }
}
void RDSetSceneDlg::GetImageFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Get Picture"));
    filePath->setText(fileName);
}

void RDSetSceneDlg::SetValue(const RDSceneData& pSceneData)
{
    ReleaseResource();
    m_data = pSceneData;
    UpdateGUI();
}
void RDSetSceneDlg::UpdateGUI()
{
    switch(m_data.m_nBackType)
    {
    case RDScene_Back_Color:
        backColor->setChecked(true);
        Color->SetColor(GetColor(m_data.backColor));
        break;
    case RDScene_Back_Picture:
        backImag->setChecked(true);
        RDImageResource* pImageResource = dynamic_cast<RDImageResource*>(RDResourceManager::GetResourceManager()->GetResource(*m_data.pImage));
        filePath->setText(pImageResource->GetPath());
        checkBox->setChecked(m_data.bStrech);
        break;
    }
}
void RDSetSceneDlg::accept()
{
    if(backColor->isChecked())
    {
        m_data.m_nBackType = RDScene_Back_Color;
        m_data.backColor = GetColor(Color->GetColor());
    }
    else if(backImag->isChecked())
    {
        RDResourceManager* pManager = RDResourceManager::GetResourceManager();
        RDImageResource* pResource = dynamic_cast<RDImageResource*>(pManager->AddImageResource(filePath->text()));

        if(pResource)
        {
            ReleaseResource();

            m_data.m_nBackType = RDScene_Back_Picture;
            m_data.pImage = const_cast<RDMd5*>(&pResource->GetMd5());
            m_data.bStrech = checkBox->isChecked();
        }
    }

    QDialog::accept();
}
