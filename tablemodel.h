#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
// #include <QTextStream>
struct Contact
{
    QString  name;
    QString  phoneNumber;
    bool  operator==(const Contact &other) const
    {
        return name == other.name && phoneNumber == other.phoneNumber;
    }
};
inline QDataStream& operator<<(QDataStream &stream, const Contact &contact)
{
    return stream << contact.name << contact.phoneNumber;
}

// inline QTextStream& operator<<(QTextStream &stream, const Contact &contact)
// {
// return stream << contact.name << contact.phoneNumber;
// }

inline QDataStream& operator>>(QDataStream &stream, Contact &contact)
{
    return stream >> contact.name >> contact.phoneNumber;
}

class TableModel: public QAbstractTableModel
{
public:
    explicit TableModel(QObject *parent = nullptr);

    // QAbstractItemModel interface

public:
    int                    rowCount(const QModelIndex&) const;

    int                    columnCount(const QModelIndex&) const;

    QVariant               data(const QModelIndex &index, int role) const;

    QVariant               headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags          flags(const QModelIndex &index) const override;

    bool                   setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool                   insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    bool                   removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    const QList<Contact> & getContacts() const;

private:
    QList<Contact>  contacts;
};

#endif // TABLEMODEL_H
