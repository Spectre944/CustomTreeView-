#ifndef CUSTOMTREEMODEL_H
#define CUSTOMTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(QObject *parent = nullptr);

    void setColumns(QStringList cols);
    void addItem(QObject* item, const QModelIndex &parentIdx);


protected:
    QStringList _columns;
    QObject* _rootItem;
    QObject* objByIndex(const QModelIndex &index) const;


    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

inline TreeModel::TreeModel(QObject *parent)
{
    _rootItem = new QObject(this);
}

inline void TreeModel::setColumns(QStringList cols)
{
    _columns = cols;
}

inline void TreeModel::addItem(QObject *item, const QModelIndex &parentIdx)
{
    beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
    item->setParent(objByIndex(parentIdx));
    endInsertRows();
}



inline QObject *TreeModel::objByIndex(const QModelIndex &index) const
{
    if(!index.isValid())
        return _rootItem;
    return static_cast<QObject*>(index.internalPointer());
}

inline QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    QObject* parentObj = objByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

inline QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    QObject* childObj = objByIndex(child);
    QObject* parentObj = childObj->parent();

    if(parentObj == _rootItem)
        return QModelIndex();
    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);

}

inline int TreeModel::rowCount(const QModelIndex &parent) const
{
    return objByIndex(parent)->children().count();
}

inline int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _columns.count();
}

inline QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole){
        return objByIndex(index)->property(_columns.at(index.column()).toUtf8());
    }
    return QVariant();
}



#endif // CUSTOMTREEMODEL_H
