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
    //Вывод файла при нажатии на имя файла в таблице
    QFile CBRN;
    CBRN.setFileName("C:\\CBRN\\Incoming\\" + CBRNClass->getTableModel()->data(index, Qt::DisplayRole).toString());

    CBRN.open(QIODevice::ReadOnly);
    QString cbrnContent = QLatin1String(CBRN.readAll());
    CBRN.close();

    ui->textEdit->clear();
    ui->textEdit->setPlainText(cbrnContent);

}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{

    qDebug() << index;
    qDebug() << CBRNClass->getTreeModel()->parent(index).data(Qt::DisplayRole).toString();

    //получаем название строк при нажатии на ветки и фильтруем по ним имена файлов, получая конечный список
    ui->tableView->resizeColumnsToContents();

    QString filter;
    QStringList filteredList;

    //Если клик был по ветке "Количество" не задаём фильтры и выводим всё
    if(index.column() != 1){

        filter.append(CBRNClass->getTreeModel()->parent(index).data(Qt::DisplayRole).toString());
        if(!filter.isEmpty())
            filter.append(" ");
        filter.append(CBRNClass->getTreeModel()->data(index, Qt::DisplayRole).toString());
    }



    filteredList = CBRNClass->_fileNames.filter(filter);

    CBRNClass->getTableModel()->updateModel(filteredList);

}


void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    //Очишаем весь фильтр и выводим все существующие файлы
    ui->tableView->resizeColumnsToContents();
    CBRNClass->getTableModel()->updateModel(CBRNClass->_fileNames);
}

