#include "tree_model.h"

#include "tree_item.h"

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(QString("Primitives"));
    //setupModelData(data.split('\n'), rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

TreeItem* TreeModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }

    return rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    
    TreeItem* parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem* childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return false;
    }

    TreeItem* p = rootItem;
    if (index.isValid()) {
        p = static_cast<TreeItem*>(index.internalPointer());
    }

    p->setData(value.toString());
    emit dataChanged(index, index);
    return true;
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void TreeModel::addTree(QString& str, const QModelIndex& parent, Drawable* d)
{

    TreeItem* p = rootItem;

    if (parent.isValid()) {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    int row = p->childCount();

    beginInsertRows(parent, row, row);

    bool success = insertRow(row, parent);

    rootItem->appendChild(new TreeItem( str, rootItem, d ));

    QModelIndex index = this->index(row, 0, parent);
    setData(index, str, Qt::DisplayRole);
    endInsertRows();
}

bool TreeModel::removeRow(int row, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row);
    
    TreeItem* p = rootItem;

    if (parent.isValid()) {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    p->removeChild(p->child(row));

    endRemoveRows();
    return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    TreeItem* p = rootItem;
    if (parent.isValid()) {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    for (int i = row; i < count; i++)
    {
        p->removeChild(p->child(0));
    }

    endRemoveRows();
    return true;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    return item->data();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data();

    return QVariant();
}