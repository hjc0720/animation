// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  the application;
// 
//        Version:  1.0
//        Created:  25/03/11 19:30:26
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================
#include <QApplication>
#include "mainwindow.h"
#include "RDMainWindow.h"
#include <QDir>
#include "HVector4f.h"

void InitEnv()
{
    QDir homeDir(QDir::home());
    if(!homeDir.cd(".ReadGoogleBook"))
    {
        homeDir.mkdir(".ReadGoogleBook");
        homeDir.cd(".ReadGoogleBook");
    }
    if(!homeDir.exists("cache"))
    {
        homeDir.mkdir("cache");
    }
    if(!homeDir.exists("temp_cache"))
    {
        homeDir.mkdir("temp_cache");
    }
    homeDir.cd("temp_cache");
    if(!homeDir.exists("Image"))
    {
        homeDir.mkdir("Image");
    }
    if(!homeDir.exists("Movie"))
    {
        homeDir.mkdir("Movie");
    }
}

int main(int argc,char *argv[])
{
    unsigned int color = 0xffffffff;
    float4 vColor(color);

    QApplication app(argc,argv);
    InitEnv();
    RDMainWindow mainWin;
    mainWin.show();
    return app.exec();
}
