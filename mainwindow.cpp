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

/*

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

*/

    CBRNClass = new CBRNView(this, "C://CBRN//Incoming");



    ui->treeView->setModel(CBRNClass->getTreeModel());

    ui->tableView->setModel(CBRNClass->getTableModel());
    ui->tableView->resizeColumnsToContents();



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QFile CBRN;
    CBRN.setFileName("C:\\CBRN\\Incoming\\" + CBRNClass->getTableModel()->data(index, Qt::DisplayRole).toString());

    CBRN.open(QIODevice::ReadOnly);
    QString cbrnContent = QLatin1String(CBRN.readAll());
    CBRN.close();

    ui->textEdit->clear();
    ui->textEdit->setPlainText(cbrnContent);


    qDebug() << CBRNClass->getTableModel()->data(index, Qt::DisplayRole);
    qDebug() << index;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{

    ui->tableView->resizeColumnsToContents();

    QString filter;
    QStringList filteredList;

    filter.append(CBRNClass->getTreeModel()->parent(index).data(Qt::DisplayRole).toString());
    if(!filter.isEmpty())
        filter.append(" ");
    filter.append(CBRNClass->getTreeModel()->data(index, Qt::DisplayRole).toString());


    qDebug() << CBRNClass->getTreeModel()->parent(index).data(Qt::DisplayRole);
    qDebug() << CBRNClass->getTreeModel()->data(index, Qt::DisplayRole);
    qDebug() << filter;

    filteredList = CBRNClass->_fileNames.filter(filter);

    //CBRNClass->getTableModel()->removeRows(0,12,QModelIndex());

    CBRNClass->getTableModel()->updateModel(filteredList);

}

