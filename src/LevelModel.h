
#ifndef LEVELMODEL_H
#define LEVELMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QJsonArray>

class LevelModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    const int columns_ {2};
    QJsonArray modelData_ {};
protected:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public:
    explicit LevelModel(QObject *parent = nullptr);
    void setModelData(const QJsonArray modelData);
};

#endif // LEVELMODEL_H
