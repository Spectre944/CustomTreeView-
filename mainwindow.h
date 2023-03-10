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

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    //TreeModel* model;
    //CustomTreeModel *model;

    CBRNView *CBRNClass;
};
#endif // MAINWINDOW_H
