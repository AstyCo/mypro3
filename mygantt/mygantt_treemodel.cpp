#include "mygantt_treemodel.h"

#include "mygantt_globalvalues.h"

#include <QSize>

Q_DECLARE_METATYPE(UtcDateTime)

GanttTreeModel::GanttTreeModel(GanttInfoNode *root,QObject * parent)
    : QAbstractItemModel(parent)
{

    m_root = root;

    if(!m_root)
        m_root = new GanttInfoNode;

    m_root->setIsExpanded(true);
    initIndexes(m_root);
}

QVariant GanttTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if (role == Qt::SizeHintRole)
    {
        QSize defSize ;
        defSize.setHeight(DEFAULT_ITEM_WIDTH);
        return defSize ;
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    GanttInfoLeaf *leaf = leafForIndex(index);
    if(leaf)
    {
        switch (index.column())
        {
        case nameField:
            return leaf->title();
        case startField:
            return leaf->start().toString();
        case finishField:
            return leaf->finish().toString();
        case durationField:
            return leaf->duration();
        default:
            Q_ASSERT(false);
        }
    }

    GanttInfoNode *node = nodeForIndex(index);
    if(node == m_root)
    {
        return QVariant();
    }

    if(node)
    {
        switch (index.column())
        {
        case nameField:
            return node->title();
        case startField:
        case finishField:
        case durationField:
            return QVariant();
        default: Q_ASSERT(false);
        }
    }

    return QVariant();
}

Qt::ItemFlags GanttTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant GanttTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{

    if (role == Qt::SizeHintRole)
    {
        QSize defSize ;
        defSize.setHeight(2*DEFAULT_ITEM_WIDTH);
        return defSize ;
    }


//    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//    {
//        switch(section)
//        {
//        case nameField:
//            return trUtf8("Название");
//        case startField:
//            return trUtf8("Начало");
//        case finishField:
//            return trUtf8("Конец");
//        case durationField:
//            return trUtf8("Длительность");
//        default:
//            return QVariant();
//        }
//    }

    return QVariant();
}

QModelIndex GanttTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    GanttInfoNode *parentNode;

    if (!parent.isValid())
    {
        parentNode = m_root;
    }
    else
        parentNode = nodeForIndex(parent);

    GanttInfoItem *childItem = parentNode->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

GanttInfoItem *GanttTreeModel::itemForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return m_root;
    }

    return static_cast<GanttInfoItem*>(index.internalPointer());
}

GanttInfoLeaf *GanttTreeModel::leafForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return NULL;
    }

    return dynamic_cast<GanttInfoLeaf*>(itemForIndex(index));
}

GanttInfoNode *GanttTreeModel::nodeForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return m_root;
    }

    return dynamic_cast<GanttInfoNode*>(itemForIndex(index));
}

void GanttTreeModel::initIndexes(GanttInfoItem *item)
{
    item->setIndex((item==m_root)?(QModelIndex()):(createIndex(item->row(),0,item)));

    GanttInfoNode *p_node = dynamic_cast<GanttInfoNode*>(item);
    if(p_node)
    {
        for(int i = 0; i < p_node->size(); ++i)
            initIndexes(p_node->child(i));
    }
}



QModelIndex GanttTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    GanttInfoItem *childItem = itemForIndex(index);
    GanttInfoNode *parentNode = childItem->parent();

    if(!parentNode)
    {
        qWarning("GanttTreeModel::parent     !parentNode");
        return QModelIndex();
    }

    if (parentNode == m_root)
    {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}

int GanttTreeModel::rowCount(const QModelIndex &parent) const
{
    GanttInfoNode *parentNode;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentNode = m_root;
    else if(!(parentNode = nodeForIndex(parent)))
        return 0;

    return parentNode->size();
}

int GanttTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
//    GanttInfoNode *parentNode;


//    if (parent.column() > 0)
//        return 0;

//    if (!parent.isValid())
//        parentNode = m_root;
//    else
//        parentNode = nodeForIndex(parent);

//    return parentNode->columnCount();

    return 4;
}

void GanttTreeModel::addItems(const QList<GanttInfoItem *> &items)
{
    m_root->append(items);
    initIndexes(m_root);
}

void GanttTreeModel::addItems(GanttInfoItem *item)
{
    m_root->append(item);
    initIndexes(m_root);
}



