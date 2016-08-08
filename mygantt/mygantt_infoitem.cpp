#include "mygantt_infoitem.h"
#include "mygantt_infonode.h"

#include "mygantt_globalvalues.h"

GanttInfoItem::GanttInfoItem(GanttInfoNode *parent)
{
    m_parent = parent;
}

QString GanttInfoItem::title() const
{
    return m_title;
}

void GanttInfoItem::setTitle(const QString &title)
{
    m_title = title;
}

int GanttInfoItem::row() const
{
    if(m_parent)
        return m_parent->indexOf(this);

    return 0;
}

int GanttInfoItem::indexOf(const GanttInfoItem* p_item) const
{
    Q_UNUSED(p_item);
    return -1;
}

QModelIndex GanttInfoItem::index() const
{
    if(!m_index.isValid())
        qDebug() << "index is not valid";

    return m_index;
}

void GanttInfoItem::setIndex(const QModelIndex &index)
{
    m_index = index;
}

void GanttInfoItem::setParent(GanttInfoNode *parent)
{
    m_parent = parent;
}

qreal GanttInfoItem::pos() const
{
    GanttInfoNode * p_parent = parent();

    if(!p_parent)
        return 0.0; // m_root

    qreal base = p_parent->pos();


    if(!p_parent->isExpanded())
        return base;

    qreal offset = (p_parent->parent())?(DEFAULT_ITEM_WIDTH):(0); // if m_root don't need extra offset
    const QList<GanttInfoItem*>& items = p_parent->m_items;
    // looking for this item, stops when found
    for(int i = 0; i < items.size(); ++i)
    {
        if( items[i] == this)
            break;

        offset += items[i]->height();
    }

    return base + offset;
}


GanttInfoNode *GanttInfoItem::parent() const
{
    return m_parent;
}

