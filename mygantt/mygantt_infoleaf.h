#ifndef GANTTINFOLEAF_H
#define GANTTINFOLEAF_H

#include "mygantt_infoitem.h"

#include "utcdatetime.h"

#include <QColor>

class GanttInfoLeaf : public GanttInfoItem
{
    Q_OBJECT

public:
    GanttInfoLeaf();

    UtcDateTime start() const;
    UtcDateTime finish() const;
    QColor getColor() const;

    long long duration() const; // in microsec

    int columnCount() const;
    qreal height() const;

signals:
    void startChanged();
    void finishChanged();
    void colorChanged();

public slots:
    void setStart(const UtcDateTime &start);
    void setFinish(const UtcDateTime &finish);
    void setColor(const QColor &value);

private:
    UtcDateTime m_start,
            m_finish;
    QColor m_color;

};

#endif // GANTTINFOITEM_H
