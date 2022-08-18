#include "tablemodel.h"

TableModel::TableModel(QObject *parent):
    QAbstractTableModel{parent}
{
}

int  TableModel::rowCount(const QModelIndex&) const
{
    return contacts.size();
}

int  TableModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant  TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if ((index.row() >= contacts.size()) || (index.row() < 0))
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        const auto &contact = contacts.at(index.row());

        switch (index.column())
        {
        case 0:

            return contact.name;
        case 1:

            return contact.phoneNumber;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant  TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:

            return tr("Name");
        case 1:

            return tr("Phone Number");
        default:
            break;
        }
    }
    else
    {
        return section + 1;
    }

    return QVariant();
}

Qt::ItemFlags  TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool  TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole))
    {
        const int  row     = index.row();
        auto       contact = contacts.value(row);

        switch (index.column())
        {
        case 0:
            contact.name = value.toString();
            break;
        case 1:
            contact.phoneNumber = value.toString();
            break;
        default:

            return false;
        }

        contacts.replace(row, contact);
        emit  dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });

        return true;
    }

    return false;
}

bool  TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        contacts.insert(position, { QString(), QString() });
    }

    endInsertRows();

    return true;
}

bool  TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        contacts.removeAt(position);
    }

    endRemoveRows();

    return true;
}

const QList<Contact>& TableModel::getContacts() const
{
    return contacts;
}
