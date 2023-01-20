#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    model = new TreeModel("C://CBRN//Incoming" ,this);
    QStringList cols;
    cols << "objectName";
    model->setColumns(cols);

    QObject* item1 = new QObject();
    item1->setObjectName("Father");

    QObject* item2 = new QObject(item1);
    item2->setProperty("objectName", "son");

    model->addItem(item1, QModelIndex());

    ui->treeView->setModel(model);
*/


    model = new CustomTreeModel();
    model->setRootPath("C://CBRN//Incoming");

    model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files);
    //model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);

    QStringList filters;
    filters <<"*.txt";
    model->setNameFilters(filters);


    ui->treeView->setModel(model);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->setRootIndex(model->index("C://CBRN//Incoming"));



}

MainWindow::~MainWindow()
{
    delete ui;
}

