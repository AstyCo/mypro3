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
    enum GanttHeaderMode
    {
        GanttDiagramMode,
        TimelineMode,

        GanttHeaderMode_count
    };

    enum GanttPrecisionMode
    {
        months1,
        days1,
        hours1,

        minutes30,
        minutes15,
        minutes1,

        seconds30,
        seconds15,
        seconds1,

        miliseconds30,
        miliseconds15,
        miliseconds1,

        microseconds30,
        microseconds15,
        microseconds1,

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

    bool setHeaderMode(const GanttHeaderMode &headerMode);
    void init();
    GanttHeaderMode headerMode() const;

    void calculateTimeMode();

    void setCurrentWidth(int currentWidth);

    void setLengthInMicroseconds(long long lengthInMicroseconds);

    const UtcDateTime &startDt() const;
    const UtcDateTime &finishDt() const;

    void setRange(UtcDateTime min, UtcDateTime max);

    static long long modeToMicrosecond(GanttPrecisionMode mode);

private:

    bool onItemsAdditionHelper(GanttInfoItem* item);
    QString textForDtStep(int step) const;


private:

    GanttHeaderMode m_headerMode;
    // Gantt mode
    UtcDateTime   m_startDt,
                  m_finishDt;
    QList<QGraphicsRectItemWithText*> m_items;
    // line mode
    qreal m_stretchFactor;
    int m_currentWidth;

    // COMMON

    UtcDateTime   m_minDt,
                  m_maxDt;

    long long m_lengthInMicroseconds;

    GanttPrecisionMode m_mode;

    GanttScene * m_scene;

    bool m_isEmpty;

    friend class GanttWidget;

};

#endif // GANTTHEADER_H
