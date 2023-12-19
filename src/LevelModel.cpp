
#include "LevelModel.h"
#include <QJsonObject>

int LevelModel::rowCount(const QModelIndex &parent) const
{
    return modelData_.size();
}

int LevelModel::columnCount(const QModelIndex &parent) const
{
    return columns_;
}

QVariant LevelModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }
    if(index.row() >= modelData_.size()){
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        const int row {index.row()};
        const int column {index.column()};
        const QJsonObject modelObject {modelData_.at(row).toObject()};
        if(!modelObject.isEmpty()){
            if(column==0){
                return modelObject.value("fileName").toString();
            }
            else if(column==1){
                return modelObject.value("warningsCount").toInt();
            }
        }
    }
    return QVariant {};
}

QVariant LevelModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal){
        if(section==0){
            return "Filename";
        }
        else if(section==1){
            return "Warnings";
        }
    }
    else{
        return section;
    }
    return QVariant{};
}

Qt::ItemFlags LevelModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsSelectable) : flags;
}

LevelModel::LevelModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

void LevelModel::setModelData(const QJsonArray modelData)
{
     beginResetModel();
     modelData_=modelData;
     endResetModel();
}

