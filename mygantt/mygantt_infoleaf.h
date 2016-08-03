#ifndef GANTTINFOLEAF_H
#define GANTTINFOLEAF_H

#include "mygantt_infoitem.h"

#include "utcdatetime.h"

#include <QColor>

class GanttInfoLeaf : public GanttInfoItem
{
public:
    GanttInfoLeaf();

    UtcDateTime start() const;
    void setStart(const UtcDateTime &start);

    UtcDateTime finish() const;
    void setFinish(const UtcDateTime &finish);

    QColor getColor() const;
    void setColor(const QColor &value);

    long long duration() const; // in microsec

    int columnCount() const;

    qreal height() const;

private:
    UtcDateTime m_start,
            m_finish;
    QColor color;

};

#endif // GANTTINFOITEM_H
