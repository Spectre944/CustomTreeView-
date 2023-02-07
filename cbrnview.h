#ifndef CBRNVIEW_H
#define CBRNVIEW_H

#include <QObject>
#include <QList>
#include <QDir>
#include <QAbstractItemModel>
#include <QModelIndex>

#include "treemodel.h"
#include "tablemodel.h"

class CBRNView : public QObject
{
    Q_OBJECT
public:
    explicit CBRNView(QObject *parent = nullptr, QString path = 0);
    QList<QString> readToStringList(QString path);
    TreeModel * getTreeModel()    {
        return _treeModel;
    }
    TableModel * getTableModel(){
        return _tableModel;
    }


    QList<QString> _fileNames;
    TreeModel* _treeModel;
    TableModel * _tableModel;

private:





signals:

};

inline CBRNView::CBRNView(QObject *parent, QString path)
{
    Q_UNUSED(parent);

    //read all file name from folder
    _fileNames.clear();
    _fileNames = readToStringList(path);

   _treeModel = new TreeModel(_fileNames);

   _tableModel = new TableModel(_fileNames.count(), 10, _fileNames);

   _tableModel->updateModel(_fileNames);


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

    fileNames.sort();

    return fileNames;
}

#endif // CBRNVIEW_H
