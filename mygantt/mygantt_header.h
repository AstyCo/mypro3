#ifndef GANTTHEADER_H
#define GANTTHEADER_H

#include "mygantt_globalvalues.h"
#include "qgraphicsrectitemwithtext.h"

#include "utcdatetime.h"

#include <QDateTime>

#include <QList>

class GanttScene;
class GanttInfoItem;
class GanttInfoLeaf;
class GanttInfoNode;

class GanttHeader : public QGraphicsItem
{
public:
    enum GanttPrecisionMode
    {
        showMonths,
        showDays,
        showHours,
        showMinutes,
        showSeconds,

        GanttPrecisionMode_count
    };


public:
    GanttHeader(QGraphicsItem *parent = 0);

    void setScene(QGraphicsScene* scene);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateHeader();

    GanttPrecisionMode mode() const;
    void setMode(const GanttPrecisionMode &mode);

    void onItemsAddition(const QList<GanttInfoItem*>& items);
    void onItemsAddition(GanttInfoItem* items);

    void initRange();

    static QString topHeaderFormat(GanttPrecisionMode mode);
    static QString secondHeaderFormat(GanttPrecisionMode mode);

    static UtcDateTime nextTopHeaderDt(const UtcDateTime& curDt,GanttHeader::GanttPrecisionMode mode);

    static UtcDateTime nextSecondHeaderDt(const UtcDateTime& curDt,GanttHeader::GanttPrecisionMode mode);

    qreal secondHeaderWidth() const;

    void clear();

    qreal dtToX(const UtcDateTime& dt) const;

private:

    bool onItemsAdditionHelper(GanttInfoItem* item);


private:
    UtcDateTime   m_minDt,
                  m_maxDt;

    UtcDateTime   m_startDt,
                  m_finishDt;

    long long m_lengthInMicroseconds;

    GanttPrecisionMode m_mode;

    QList<QGraphicsRectItemWithText*> m_items;
    GanttScene * m_scene;

    bool m_isEmpty;

};

#endif // GANTTHEADER_H
