// =====================================================================================
// 
//       Filename:  RDPropertySheet.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 10:38:31 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_PROPERTY_SHEET_INC
#define  RD_PROPERTY_SHEET_INC
#include <QDockWidget>

class QScrollArea;
class QVBoxLayout;
class RDCell;
class RDPropertySheet : public QDockWidget
{
    Q_OBJECT;
public:
    RDPropertySheet(int nWidth,QWidget* parent = 0);
    ~RDPropertySheet();
public slots:
    void PropertyChanged(RDCell** pCellArray,int nCount);
protected:
    QScrollArea* m_pMainWidget;
    QVBoxLayout* m_pLayOut;
};
#endif   // ----- #ifndef rdpropertysheet_INC  -----
