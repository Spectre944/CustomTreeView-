#ifndef CBRNVIEW_H
#define CBRNVIEW_H

#include <QObject>
#include <QList>
#include <QDir>
#include <QAbstractItemModel>
#include <QModelIndex>

#include "CustomTreeModel2.h"

class CBRNView : public QObject
{
    Q_OBJECT
public:
    explicit CBRNView(QObject *parent = nullptr, QString path = 0, int typeSorting = 0);
    QList<QString> readToStringList(QString path);
    TreeModel getModel()    {
        return _model;
    }


    QList<QString> _fileNames;
    TreeModel* _model;

private:





signals:

};

inline CBRNView::CBRNView(QObject *parent, QString path, int typeSorting)
{
    //read all file name from folder
    _fileNames.clear();
    _fileNames = readToStringList(path);

   _model = new TreeModel();

    QStringList cols;

    switch (typeSorting) {
        //CBRN by number
        case 0:      cols << "CBRN" << "Тип" << "Кількість";    break;

        //CBRN by type
        case 1:      cols << "Тип" << "CBRN" << "Кількість";   break;

    }

    _model->setColumns(cols);

    QObject* item1 = new QObject();
    item1->setObjectName("Father");

    QObject* item2 = new QObject(item1);
    item2->setProperty("CBRN", "CBRN1");

    QObject* item3 = new QObject(item2);
    item3->setProperty("Тип", "NUC");

    _model->addItem(item1, QModelIndex());

}

inline QList<QString> CBRNView::readToStringList(QString path)
{
    QDir directory(path);
    QList<QString> fileNames;
    fileNames.clear();

    QStringList files = directory.entryList(QStringList() << "*.txt" << "*.TXT",QDir::Files);
    foreach(QString filename, files) {

        fileNames.append(filename);
    }

    return fileNames;
}

#endif // CBRNVIEW_H
