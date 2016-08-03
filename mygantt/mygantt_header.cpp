#include "mygantt_header.h"

#include "mygantt_scene.h"
#include "mygantt_infoleaf.h"
#include "mygantt_infonode.h"


#include <QGraphicsRectItem>

#include <QGraphicsView>
#include <QPainter>
#include <QBrush>
#include <QDebug>

GanttHeader::GanttHeader(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{

    m_scene = NULL;

    m_minDt = UtcDateTime(QDate(2015,1,29));
    m_maxDt = UtcDateTime(QDate(2015,2,2));
//    m_maxDt = UtcDateTime(QDate(2016,1,22));

    m_mode = showMonths;

    m_isEmpty = true;

    setZValue(10);


}

void GanttHeader::setScene(QGraphicsScene *scene)
{
    m_scene = dynamic_cast<GanttScene*>(scene);
    if(!m_scene)
    {
        qWarning("GanttHeader::setScene no m_scene");
        return;
    }
    m_scene->addItem(this);
    updateHeader();
}

QRectF GanttHeader::boundingRect() const
{
    return QRectF();
}

void GanttHeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //
}

void GanttHeader::updateHeader()
{
    if(!m_scene)
    {
        qWarning("GanttHeader::updateHeader no m_scene");
        return;
    }

    if(m_isEmpty)
        return;


    clear();

    initRange();

    qDebug() << "start: "
             << m_startDt
             << "finish: "
             << m_finishDt;

    UtcDateTime dt = m_startDt;


    qreal sumWidth = 0,
            curOffset = 0;

    while(dt<m_finishDt)
    {
        UtcDateTime nextDt = nextTopHeaderDt(dt,m_mode);

        qreal topHeaderStartsAt = curOffset;
        QGraphicsRectItemWithText *p_topHeaderItem =  new QGraphicsRectItemWithText( dt.toString(topHeaderFormat(m_mode))
                                                                          ,QRectF()
                                                                          ,this);

        while(dt<nextDt)
        {
            QGraphicsRectItemWithText *p_secondHeadeItem = new QGraphicsRectItemWithText( dt.toString(secondHeaderFormat(m_mode))
                                                                              ,QRectF(curOffset,DEFAULT_ITEM_WIDTH,secondHeaderWidth(),DEFAULT_ITEM_WIDTH)
                                                                              ,this);

            if(m_mode == showDays)
            {
                Qt::DayOfWeek d = (Qt::DayOfWeek)dt.date().dayOfWeek();
                if (((d == Qt::Saturday) || (d == Qt::Sunday)))
                  p_secondHeadeItem->setTextColor( QColor(230,0,0) );
            }

            p_secondHeadeItem->setParentItem(p_topHeaderItem);
            m_items.append(p_secondHeadeItem);
            curOffset+=secondHeaderWidth();

            dt = nextSecondHeaderDt(dt,m_mode);
        }

        p_topHeaderItem->setRect(QRectF(topHeaderStartsAt,0,curOffset-topHeaderStartsAt,DEFAULT_ITEM_WIDTH));

        m_items.append(p_topHeaderItem);
    }

    m_scene->updateWidth(curOffset);
}

GanttHeader::GanttPrecisionMode GanttHeader::mode() const
{
    return m_mode;
}

void GanttHeader::setMode(const GanttPrecisionMode &mode)
{
    if(m_mode == mode)
        return;
    m_mode = mode;
    updateHeader();
}

void GanttHeader::onItemsAddition(const QList<GanttInfoItem *> &items)
{
    bool newRange = false;
    foreach(GanttInfoItem *item, items)
    {
        newRange|= onItemsAdditionHelper(item);
    }
    if(newRange)
        updateHeader();
}

void GanttHeader::onItemsAddition(GanttInfoItem* items)
{
    if(onItemsAdditionHelper(items))
    {
        updateHeader();
    }
}

void GanttHeader::initRange()
{
    switch(m_mode)
    {
    case showSeconds:
        m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),m_minDt.minute())));
        m_finishDt = UtcDateTime(
                    (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                    ,QTime(m_maxDt.hour(),m_maxDt.minute()).addSecs(SECONDS_IN_MINUTE)
                    ).addMicroseconds(-1);

        break;
    case showMinutes:
        m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),0)));
        m_finishDt = UtcDateTime(
                    (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                    ,QTime(m_maxDt.hour(),0).addSecs(SECONDS_IN_HOUR)
                    ).addMicroseconds(-1);

        break;
    case showHours:
        m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day())));
        m_finishDt = UtcDateTime(
                    (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day())).addDays(1)
                                 ).addMicroseconds(-1);

        break;
    case showDays:
        m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),1)));
        m_finishDt = UtcDateTime(
                    (QDate(m_maxDt.year(),m_maxDt.month(),1)).addMonths(1)
                    ).addMicroseconds(-1);

        break;
    case showMonths:
        m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),1,1)));
        m_finishDt = UtcDateTime(
                    (QDate(m_maxDt.year(),1,1)).addYears(1)
                    ).addMicroseconds(-1);

        break;
    default:
        break;
    }

    m_lengthInMicroseconds = m_startDt.microsecondsTo(m_finishDt);
    qDebug()  << "m_lengthInMicroseconds = " << QString::number(m_lengthInMicroseconds);
}

QString GanttHeader::topHeaderFormat(GanttHeader::GanttPrecisionMode mode)
{
    switch (mode)
    {
    case showSeconds:
        return ("ddd d MMM yy, hh:mm");
        break;
    case showMinutes:
        return ("ddd d MMM yy, hh:00");
        break;
    case showHours:
        return ("ddd d MMM yy");
        break;
    case showDays:
        return ("MMM yyyy");
        break;
    case showMonths:
        return ("yyyy");
        break;
    default:
        break;
    }
}

QString GanttHeader::secondHeaderFormat(GanttHeader::GanttPrecisionMode mode)
{
    switch (mode)
    {
    case showSeconds:
        return ("s");
        break;
    case showMinutes:
        return ("m");
        break;
    case showHours:
        return ("h");
        break;
    case showDays:
        return ("d");
        break;
    case showMonths:
        return ("MMM");
        break;
    default:
        break;
    }
}

UtcDateTime GanttHeader::nextTopHeaderDt(const UtcDateTime &curDt,GanttHeader::GanttPrecisionMode mode)
{
    switch(mode)
    {
    case showSeconds:
        return curDt.addSecs(SECONDS_IN_MINUTE);
        break;
    case showMinutes:
        return curDt.addSecs(SECONDS_IN_HOUR);
        break;
    case showHours:
        return curDt.addDays(1);
        break;
    case showDays:
        return curDt.addMonths(1);
        break;
    case showMonths:
        return curDt.addYears(1);
        break;
    default:
        break;
    }

    return curDt;
}

UtcDateTime GanttHeader::nextSecondHeaderDt(const UtcDateTime &curDt,GanttHeader::GanttPrecisionMode mode)
{
    switch(mode)
    {
    case showSeconds:
        return curDt.addSecs(1);
        break;
    case showMinutes:
        return curDt.addSecs(SECONDS_IN_MINUTE);
        break;
    case showHours:
        return curDt.addSecs(SECONDS_IN_HOUR);
        break;
    case showDays:
        return curDt.addDays(1);
        break;
    case showMonths:
        return curDt.addMonths(1);
        break;
    default:
        break;
    }

    return curDt;
}

qreal GanttHeader::secondHeaderWidth() const
{
    switch(m_mode)
    {
    case showSeconds:
    case showMinutes:
    case showHours:
    case showDays:
        return DEFAULT_ITEM_WIDTH;
        break;
    case showMonths:
        return DEFAULT_MONTH_WIDTH;
        break;
    default:
        break;
    }
    return 0;
}

void GanttHeader::clear()
{
    foreach(QGraphicsRectItemWithText* item, m_items)
    {
        m_scene->removeItem(item);
        m_items.removeOne(item);
        delete item;
    }
}

qreal GanttHeader::dtToX(const UtcDateTime &dt) const
{
    qreal sceneW = m_scene->width();

    return ((1.0)*m_startDt.microsecondsTo(dt))/m_lengthInMicroseconds * sceneW;

}

bool GanttHeader::onItemsAdditionHelper(GanttInfoItem *item)
{
    GanttInfoLeaf *leaf = dynamic_cast<GanttInfoLeaf*>(item);
    bool newRange = false;
    if(leaf)
    {
        if(m_isEmpty)
        {
            m_isEmpty = false;
            m_minDt = leaf->start();
            m_maxDt = leaf->finish();
        }

        if(m_maxDt<leaf->finish())
        {
            m_maxDt = leaf->finish();
            newRange = true;
        }
        if(m_minDt>leaf->start())
        {
            m_minDt=leaf->start();
            newRange = true;
        }
    }
    else
    {
        GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(item);
        if(node)
        {
            foreach(GanttInfoItem* item, node->m_items)
            {
                newRange |= onItemsAdditionHelper(item);
            }
        }
    }

    return newRange;
}

