// =====================================================================================
// 
//       Filename:  RDCell.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/27/2011 12:02:04 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Huang Jianchao (), hjc0720@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  RD_CELL_INC
#define  RD_CELL_INC

#include <QWidget>
#include <QVBoxLayout>

class QString;
class RDMd5;
class RDCell : public QWidget
{
    Q_OBJECT
public:
    RDCell(const QString& title, QWidget* parent);
    virtual const RDMd5& GetCellMd5() = 0 ;
    void AddWidget(QWidget* pWidget){m_pMainLayout->addWidget(pWidget);}
    void AddLayout(QLayout* pLayout){m_pMainLayout->addLayout(pLayout);}
protected:
    size_t GetCurChangeIndex()const{return  m_nCurChangeIndex;}
    void setCurChangeIndex(size_t nIndex){m_nCurChangeIndex = nIndex;}
signals:
    void    CellChanged(const RDMd5& pCell,int nIndex);
protected slots:
    void    ShowLayout(bool checked);
    void    CellChange();
protected:
    QWidget*     m_pMainWidget;
    QVBoxLayout* m_pMainLayout;
    size_t       m_nCurChangeIndex;
};
#endif   // ----- #ifndef rdcell_INC  -----
