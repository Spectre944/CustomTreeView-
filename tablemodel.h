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
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool removeRows(int position, int rows, const QModelIndex &index);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setHeaderData(Qt::Orientation orientation, const QList<QVariant> &value, int role);
    virtual void sort(int column, Qt::SortOrder order);
    virtual bool insertRows(int row, int count, const QModelIndex &parent);
    bool insertRowData(int row, const QList<QVariant> &value, const QModelIndex &index);



    void updateModel(const QList<QString> &data);


private:

    int _rowCount = 0;
    int _colCount = 0;
    QList<QVariant> headerNames;


protected:
    QHash<QModelIndex, QVariant> _cellData;

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
    QList<QVariant> headers;

    //ALPHA, DELTA, FOXTROT, GOLF, HOTEL, INDIA, INDIAR, INDIAB, INDIAC,
    headers << "Файл" << "ALPHA/" << "DELTA/" << "FOXTROT/" << "GOLF/" << "HOTEL/" << "INDIA/" << "INDIAR/" << "INDIAB/" << "INDIAC/";
    setHeaderData(Qt::Horizontal, headers, Qt::EditRole);


    _cellData.clear();



    //dont build model if empty
    if(!data.isEmpty()){

        //remove all rows before update
        removeRows(0, rowCount(QModelIndex()),QModelIndex());
        insertRows(0,data.count(),QModelIndex());
    }


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

            QList<QVariant> witeData;

            //Имя файла нулевым столбцом
            witeData << data[number];

            //Ищем строки, обрезаем и заполняем, если ничего не найдено в строке ставим NULL
            for(int i = 1; i < headers.count(); i++){

                cbrnRow = headers.at(i).toString();
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

            //внесение данных
            for(int i = 0; i < witeData.count(); i++)
                setData(index(number,i),witeData.at(i),Qt::DisplayRole);

        }

    ++number;
    }


}

inline bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, count - 1);

    _rowCount = count;

    endInsertRows();
    return true;
}

inline void TableModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);

    QStringList DalaList;
    //QList<QDateTime> TimeList;



    emit layoutAboutToBeChanged();

    if(headerData(column,Qt::Horizontal, Qt::DisplayRole).toString() == "DELTA/"){


        //read data to arrays
        for(int i = 0; i < _rowCount; i++){
            //DalaList = _cellData.value().toStringList();
        }

        if(order == Qt::AscendingOrder){

            qDebug() <<_cellData.begin().value();


        }
        else{

        }

    }
    else{

        if(order == Qt::AscendingOrder){
            //std::sort(_cellData.
        }
        else{
             //std::sort(_cellData.begin(), _cellData.end(), std::greater<QString>());
        }
    }



    emit layoutChanged();
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
    //headerNames.clear();
    headerNames << value;
    return true;
}


inline bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{

    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), position, rows - 1);

    _rowCount = 0;

    endRemoveRows();
    return true;
}









#endif // TABLEMODEL_H
