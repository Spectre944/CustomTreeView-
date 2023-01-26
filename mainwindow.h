#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>


//#include "CustomTreeModel.h"
//#include "CustomTreeModel2.h"
#include "cbrnview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //TreeModel* model;
    //CustomTreeModel *model;

    CBRNView *CBRNClass;
};
#endif // MAINWINDOW_H
