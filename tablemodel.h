#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QFile>

class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(int row, int col, const QList<QString> &data, QObject *parent = nullptr);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool removeRows(int row, int count, const QModelIndex &parent);


    void updateModel(const QList<QString> &data);


private:

    int _rowCount = 0;
    int _colCount = 0;


protected:
    QHash<QModelIndex, QVariant> _cellData;

};

inline TableModel::TableModel(int row, int col,const QList<QString> &data, QObject *parent)
    : QAbstractTableModel(parent), _rowCount(row), _colCount(col)
{

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
        return _cellData.value(index, QVariant());
    }
    return QVariant();
}

inline Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEditable;

}

inline bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        _cellData.insert(index, value);
        emit dataChanged(index, index);
    }
    return false;
}




inline void TableModel::updateModel(const QList<QString> &data)
{

    int number = 0;
    QString cbrnContent = 0;
    QStringList result;
    QFile cbrnFile;
    QString cbrnRow = 0;
    while (number < data.count()) {


        //Копируем нумерованые элементы в отдельную строку и работаеми с ними
        const QString lineData = data[number];

        if (!lineData.isEmpty()) {

            //Путь к файлу
            QString fileName = "C:\\CBRN\\Incoming\\" + lineData;

            //Если файла не существует пропуск
            if(!QFile().exists(fileName)){
                ++number;
                continue;

            }

            //Открытие, копирование и закрытие файла
            cbrnFile.setFileName(fileName);
            cbrnFile.open(QIODevice::ReadOnly);
            cbrnContent = QLatin1String(cbrnFile.readAll());
            cbrnFile.close();
            const QStringList columnStrings = cbrnContent.split(QLatin1Char('\n'), Qt::SkipEmptyParts);


            setData(index(number, 0), data[number], Qt::DisplayRole);

            //ALPHA, DELTA, FOXTROT, GOLF, HOTEL, INDIA, INDIAR, INDIAB, INDIAC,

            cbrnRow = "ALPHA/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0].remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 1), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "DELTA/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 2), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "FOXTROT/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 3), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "GOLF/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 4), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "HOTEL/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 5), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "INDIA/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 6), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "INDIAR/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 7), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "INDIAB/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 8), cutInfo, Qt::DisplayRole);
            }

            cbrnRow = "INDIAC/";
            result = columnStrings.filter(cbrnRow);
            if(!result.isEmpty()){
                QString cutInfo = result[0];
                cutInfo.remove(cbrnRow);
                cutInfo.remove(QString("//"));
                setData(index(number, 9), cutInfo, Qt::DisplayRole);
            }

        }

    ++number;
    }


}

inline bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, count);

//    for (int row = 0; row < _rowCount; ++row) {
//        setData(index(row,0),"",Qt::DisplayRole);
//        setData(index(row,1),"",Qt::DisplayRole);
//        setData(index(row,2),"",Qt::DisplayRole);
//        setData(index(row,3),"",Qt::DisplayRole);
//        setData(index(row,4),"",Qt::DisplayRole);
//        setData(index(row,5),"",Qt::DisplayRole);
//        setData(index(row,6),"",Qt::DisplayRole);
//        setData(index(row,7),"",Qt::DisplayRole);
//        setData(index(row,8),"",Qt::DisplayRole);
//        setData(index(row,9),"",Qt::DisplayRole);
//    }



    endRemoveRows();
    return true;
}









#endif // TABLEMODEL_H
