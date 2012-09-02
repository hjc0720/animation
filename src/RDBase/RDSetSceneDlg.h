// =====================================================================================
// 
//       Filename:  RDSetSceneDlg.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/05/11 21:59:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDSETSCENEDLG_INC
#define  RDSETSCENEDLG_INC
#include <QDialog>
#include "RDScene.h"
class QRadioButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class RDColorWidget;

class RDSetSceneDlg : public QDialog
{
    Q_OBJECT;
public:
    RDSetSceneDlg(QWidget * parent = 0);
    ~RDSetSceneDlg();
    void SetValue(const RDSceneData& pSceneData);
    const RDSceneData& GetValue()const{return m_data;}
    void accept();
protected:
    void UpdateGUI();
    void ReleaseResource();
protected slots:
    void GetImageFile();
protected:
    RDSceneData m_data;
    //gui data
protected:
    QRadioButton *backColor;
    RDColorWidget *Color;
    QRadioButton *backImag;
    QLineEdit *filePath;
    QPushButton *getFile;
    QCheckBox *checkBox;
};
#endif   // ----- #ifndef rdsetscenedlg_INC  -----
