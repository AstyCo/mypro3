#include "mygantt_infoleaf.h"
#include "mygantt_treemodel.h"

#include "mygantt_globalvalues.h"

GanttInfoLeaf::GanttInfoLeaf()
{

}


UtcDateTime GanttInfoLeaf::start() const
{
    return m_start;
}

void GanttInfoLeaf::setStart(const UtcDateTime &start)
{
    m_start = start;
}
UtcDateTime GanttInfoLeaf::finish() const
{
    return m_finish;
}

void GanttInfoLeaf::setFinish(const UtcDateTime &finish)
{
    m_finish = finish;
}
QColor GanttInfoLeaf::getColor() const
{
    return color;
}

void GanttInfoLeaf::setColor(const QColor &value)
{
    color = value;
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



