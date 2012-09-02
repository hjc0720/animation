// =====================================================================================
// 
//       Filename:  RDMainWindow.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  25/03/11 20:16:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RDMAINWINDOW_INC
#define  RDMAINWINDOW_INC
#include "mainwindow.h"
class RDMainWindow : public MainWindow
{
    Q_OBJECT
public:
    RDMainWindow(QWidget *parent = 0);
    ~RDMainWindow();
};

#endif   // ----- #ifndef rdmainwindow_INC  -----
