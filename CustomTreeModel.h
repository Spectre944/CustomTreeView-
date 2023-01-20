#ifndef CUSTOMTREEMODEL_H
#define CUSTOMTREEMODEL_H

#include <QObject>
#include <QFileSystemModel>
#include <QStyledItemDelegate>

class CustomTreeModel : public QFileSystemModel, public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomTreeModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual bool hasChildren(const QModelIndex &parent) const;

    // QStyledItemDelegate interface
protected:
    virtual void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
};


inline CustomTreeModel::CustomTreeModel(QObject *parent)
    : QFileSystemModel{parent}
{

}

inline bool CustomTreeModel::hasChildren(const QModelIndex &parent) const
{
    QString tmp = filePath(parent);

    return QDir(filePath(parent)).count() > 2 ;
}

inline void CustomTreeModel::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{


    if(index.column() != 0)
        return;

    if(index.model()->hasChildren(index)){
        if(index.model()->canFetchMore(index)){
            int i = 0;
        option->text += " (" + QString::number(index.model()->rowCount(index)) + ")";

        }
    }
}

#endif // CUSTOMTREEMODEL_H
