#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>

#include <QDebug>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    virtual void appendChild(TreeItem *child);

    virtual TreeItem *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual int row() const;
    virtual TreeItem *parentItem();

private:
    QList<TreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

inline TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{}

inline TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

inline void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

inline TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

inline int TreeItem::childCount() const
{
    return m_childItems.count();
}

inline int TreeItem::columnCount() const
{
    return m_itemData.count();
}

inline QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

inline TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

inline int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(const QList<QString> &data, QObject *parent = nullptr);
    ~TreeModel();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    virtual void setupModelData(const QStringList &lines, TreeItem *parent);

    TreeItem *rootItem;
};


inline TreeModel::TreeModel(const QList<QString> &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem({tr("CBRN"), tr("Кількість")});
    setupModelData(data, rootItem);
}

inline TreeModel::~TreeModel()
{
    delete rootItem;
}

inline int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

inline QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

inline Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

inline QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

inline QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

inline QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

inline int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


inline void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem *> parents;
    QList<QVariant> columnData;
    QString CBRNTree = 0, TypeTree = 0;
    int CBRNAmmount = 0, TypeAmount = 0;

    parents << parent;

    int number = 0;

    while (number < lines.count()) {

        //Копируем нумерованые элементы в отдельную строку и работаеми с ними
        const QString lineData = lines[number];

        if (!lineData.isEmpty()) {

            // Разделяет строку по пробелам и запихивает их в QStringList
            const QStringList columnStrings = lineData.split(QLatin1Char(' '), Qt::SkipEmptyParts);

                //Проверка на невалидный файл, у файла как минимум должны быть два слова разделённых пробелом
                if(columnStrings.count() < 2){
                    ++number;
                    continue;

                }


                if(number!= 0 && CBRNTree != columnStrings[0])
                    parents << parent;

                //Если предведущий элемент не равен нынешнему, то начинаем новую ветку привязываясь к RootItem
                if(CBRNTree != columnStrings[0]){

                    //Перезаписываем что сведетельствует что мы находимся в ветке
                    CBRNTree = columnStrings[0];

                    //Подсчёт кол-во донесений определённого типа
                    for (int i = 0; i < lines.count(); i++)
                        CBRNAmmount += lines.at(i).contains(CBRNTree);

                    //            CBRN 1345      Кол-во
                    columnData << CBRNTree << CBRNAmmount;

                    //Добавляем ребёнка к RootItem
                    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

                    //Переход на новую строку
                    parents << parents.last()->child(parents.last()->childCount()-1);

                    columnData.clear();
                    CBRNAmmount = 0;
                }

                if(columnStrings.at(1).isEmpty())
                    break;

                //Если не равняется начинаем новую ветку со ссылкой на родителя выше
                if(TypeTree != columnStrings[1]){

                    //Перезаписываем, если совпадение то остаёмся в этой ветке
                    TypeTree = columnStrings[1];

                    //Подсчёт кол-во донесений определённого типа
                    for (int i = 0; i < lines.count(); i++)
                        TypeAmount += lines.at(i).contains(CBRNTree + " " + TypeTree);

                    //            Type            Кол-во
                    columnData << TypeTree << TypeAmount;

                    //Добавляем Тип к СBRN
                    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

                    columnData.clear();
                    TypeAmount = 0;
                }

        }
        ++number;
    }
}


#endif // TREEMODEL_H
