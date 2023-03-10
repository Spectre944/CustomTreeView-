#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDateTime>

class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(int row, int col, const QList<QString> &data, QObject *parent = nullptr);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool removeRows(int position, int rows, const QModelIndex &index);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual void sort(int column, Qt::SortOrder order);
    virtual bool insertRows(int row, int count, const QModelIndex &parent);
    using QAbstractTableModel::setData;
    virtual bool setData(const QModelIndex &index, const QVariantList &value, int role);
    using QAbstractTableModel::setHeaderData;
    virtual bool setHeaderData(Qt::Orientation orientation, const QList<QVariant> &value, int role);

    void updateCBRNModel(const QList<QString> &data);
    void clearCBRNModel();


private:

    int _rowCount = 0;
    int _colCount = 0;
    QVariantList headerNames;


protected:

    QList<QVariantList> _cellData;

};

inline TableModel::TableModel(int row, int col,const QList<QString> &data, QObject *parent)
    : QAbstractTableModel(parent), _rowCount(row), _colCount(col)
{
     Q_UNUSED(data);
}

inline int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rowCount;
}

inline int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _colCount;
}

inline QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        return _cellData.at(index.row()).at(index.column());
    }
    return QVariant();
}

inline Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEditable;

}

inline bool TableModel::setData(const QModelIndex &index, const QVariantList &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        _cellData.insert(index.row(),value);
        emit dataChanged(index, index);
    }
    return false;
}


inline void TableModel::updateCBRNModel(const QList<QString> &data)
{

    int number = 0;
    QString cbrnContent = 0;
    QStringList result;
    QFile cbrnFile;
    QString cbrnRow = 0;
    QDateTime FileCreatedTime;

    //clear all before update
    clearCBRNModel();

    //insert new row depending how much data we have
    insertRows(0, data.count(),QModelIndex());

    while (number < data.count()) {

        //???????????????? ?????????????????????? ???????????????? ?? ?????????????????? ???????????? ?? ?????????????????? ?? ????????
        const QString lineData = data[number];

        if (!lineData.isEmpty()) {

            //???????? ?? ??????????
            QString fileName = "C:\\CBRN\\Incoming\\" + lineData;

            //????????????????, ?????????????????????? ?? ???????????????? ??????????
            cbrnFile.setFileName(fileName);
            cbrnFile.open(QIODevice::ReadOnly);
            cbrnContent = QLatin1String(cbrnFile.readAll());
            cbrnFile.close();
            const QStringList columnStrings = cbrnContent.split(QLatin1Char('\n'), Qt::SkipEmptyParts);                  

            QList<QVariant> witeData;

            //?????? ?????????? ?????????????? ????????????????
            witeData << data[number];

            //?????????????????? ?????? ?????????? ?? ????????????
            QStringList fileStringList =  data[number].split(" ");

            //?????????????????? ???? ?????????? ???? ???????????? ?? ?????????? ???????????????? ??????????
            if(fileStringList.count() > 3){

                QString time = fileStringList[3];
                int secFromEpoc = time.split(".")[0].toInt();
                FileCreatedTime.setSecsSinceEpoch( secFromEpoc );

            }
            else{
                FileCreatedTime.setSecsSinceEpoch( 0 );
            }

            //???????????????????? ???????? ???????????????? ?? ??????????????
            witeData << FileCreatedTime;

            //???????? ????????????, ???????????????? ?? ??????????????????, ???????? ???????????? ???? ?????????????? ?? ???????????? ???????????? NULL
            for(int i = 2; i < headerNames.count(); i++){

                cbrnRow = headerNames.at(i).toString();
                result = columnStrings.filter(cbrnRow);

                if(!result.isEmpty()){
                    QString cutInfo = result[0].remove(cbrnRow);
                    cutInfo.remove(QString("//"));
                    witeData << cutInfo;
                }
                else{
                    witeData << NULL;
                }

            }

            setData(index(number,0), witeData, Qt::DisplayRole);

        }

    ++number;
    }


}

inline void TableModel::clearCBRNModel()
{
    //if model is empty - don`t do anything
    if(rowCount(QModelIndex()) <= 0)
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);

    _cellData.clear();
    _rowCount = 0;

    endRemoveRows();
}

inline bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{

    Q_UNUSED(parent);

    //if model is empty - don`t do anything
    if(count <= 0)
        return false;

    beginInsertRows(QModelIndex(), row, count - 1);

    _rowCount = count;

    endInsertRows();
    return true;

}

inline void TableModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged({QModelIndex()}, QAbstractItemModel::VerticalSortHint);

    std::sort(_cellData.begin(),_cellData.end(),[column,order](const QVariantList& a, const QVariantList& b)->bool{
        const auto aV = a.at(column);
        const auto bV = b.at(column);

        if(order == Qt::AscendingOrder)
            return aV==bV ?  a.at(2)<b.at(2) : aV<bV;

       return aV==bV ? a.at(2)>b.at(2) : aV>bV;
    });

    emit layoutChanged({QModelIndex()}, QAbstractItemModel::VerticalSortHint);
}

inline QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        if(!headerNames.at(section).isNull())
            return headerNames.at(section);
    }
    else{
        return section + 1;
    }

    return QVariant();

}

inline bool TableModel::setHeaderData(Qt::Orientation orientation, const QList<QVariant> &value, int role)
{
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    headerNames << value;
    return true;
}

inline bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{

    Q_UNUSED(index);

    if(_rowCount == 0)
        return true;

    beginRemoveRows(QModelIndex(), position, rows - 1);

    _rowCount = 0;

    endRemoveRows();
    return true;
}



#endif // TABLEMODEL_H
