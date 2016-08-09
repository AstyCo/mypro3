#include "mygantt_infoleaf.h"
#include "mygantt_treemodel.h"

#include "mygantt_globalvalues.h"

GanttInfoLeaf::GanttInfoLeaf()
{
    m_start = m_finish = UtcDateTime();


    connect(this,SIGNAL(startChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(finishChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(colorChanged()),this,SIGNAL(changed()));
}


UtcDateTime GanttInfoLeaf::start() const
{
    return m_start;
}

void GanttInfoLeaf::setStart(const UtcDateTime &start)
{
    if(start == m_start)
        return;

    m_start = start;
    emit startChanged();
}
UtcDateTime GanttInfoLeaf::finish() const
{
    return m_finish;
}

void GanttInfoLeaf::setFinish(const UtcDateTime &finish)
{
    if(finish == m_finish)
        return;

    m_finish = finish;
    emit finishChanged();
}
QColor GanttInfoLeaf::getColor() const
{
    return m_color;
}

void GanttInfoLeaf::setColor(const QColor &value)
{
    if(value == m_color)
        return;

    m_color = value;
    emit colorChanged();
}

long long GanttInfoLeaf::duration() const
{
    return m_start.microsecondsTo(m_finish);
}

int GanttInfoLeaf::columnCount() const
{
    return GanttTreeModel::Fields_count;
}

qreal GanttInfoLeaf::height() const
{
    return DEFAULT_ITEM_WIDTH;
}



