#include "mygantt_header.h"

#include "mygantt_scene.h"
#include "mygantt_infoleaf.h"
#include "mygantt_infonode.h"
#include "mygantt_widget.h"

#include "mygantt_graphicsview.h"


#include <QGraphicsRectItem>

#include <QGraphicsView>
#include <QPainter>
#include <QBrush>
#include <QDebug>

GanttHeader::GanttHeader(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{

    m_scene = NULL;
    m_widget = NULL;

    m_mode = months1;
    setHeaderMode(TimelineMode);
    m_currentWidth = 0;
    m_stretchFactor = 1;

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

    m_widget = dynamic_cast<GanttWidget*>(m_scene->parent());

    init();
    m_scene->addItem(this);
    updateHeader();
}

QRectF GanttHeader::boundingRect() const
{
    return QRectF(0,0,m_currentWidth,DEFAULT_HEADER_HEIGHT);
}

void GanttHeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //

    if(m_headerMode == GanttHeader::TimelineMode)
    {
        QRect rect = boundingRect().toRect();
        QRect headerRect(rect.left(),rect.top(),rect.width(),DEFAULT_HEADER_HEIGHT);
        // Header background
        QLinearGradient linearGradient(QPointF(rect.left(),rect.top()),QPointF(rect.left(),rect.top() + DEFAULT_HEADER_HEIGHT));
        linearGradient.setColorAt(0, Qt::cyan);
        linearGradient.setColorAt(0.4, Qt::white);
        linearGradient.setColorAt(0.6, Qt::white);
        linearGradient.setColorAt(1, Qt::cyan);
        painter->fillRect(headerRect, linearGradient);
        // Center horizontal line
        painter->drawLine(QPointF(0,DEFAULT_ITEM_HEIGHT),QPointF(rect.right(),DEFAULT_ITEM_HEIGHT));
        // Little and big hatches(with text)
        qreal curOffset = MIN_WIDTH_FOR_TIME_VISUALIZING/2;

        UtcDateTime dt = m_startDt;

        do
        {
            if(isDrawn(dt,m_mode))
            {
                painter->drawLine(QPointF(curOffset,DEFAULT_ITEM_HEIGHT),QPointF(curOffset,DEFAULT_ITEM_HEIGHT*(3.0/2))); // big hatch
                QFont dtFont("Goudy Old Style", 8);
                painter->setFont(dtFont);
                painter->drawText(QRectF(-MIN_WIDTH_FOR_TIME_VISUALIZING/2 + curOffset, DEFAULT_ITEM_HEIGHT*(3.0/2),
                                         MIN_WIDTH_FOR_TIME_VISUALIZING,DEFAULT_ITEM_HEIGHT/2)
                                  ,dt.toString(formatForMode(m_mode)),QTextOption(Qt::AlignCenter));
            }
            else
                painter->drawLine(QPointF(curOffset,DEFAULT_ITEM_HEIGHT),QPointF(curOffset,DEFAULT_ITEM_HEIGHT*(5.0/4))); // little hatch

            dt=dt.addMicroseconds(modeToMicrosecond(m_mode,dt.date()) / modeToSegmentCount(m_mode,dt.date()));
            curOffset=dtToX(dt);
        }
        while(dt <= m_finishDt);
    }

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

    if(m_headerMode == GanttDiagramMode)
    {
        clear();

        initRange();

        UtcDateTime dt = m_startDt;


        qreal curOffset = 0;

        while(dt<m_finishDt)
        {
            UtcDateTime nextDt = nextTopHeaderDt(dt,m_mode);

            qreal topHeaderStartsAt = curOffset;
            QGraphicsRectItemWithText *p_topHeaderItem =  new QGraphicsRectItemWithText( dt.toString(topHeaderFormat(m_mode))
                                                                                         ,QRectF()
                                                                                         ,this);

            while(dt<nextDt)
            {
                QGraphicsRectItemWithText *p_secondHeaderItem = new QGraphicsRectItemWithText( dt.toString(secondHeaderFormat(m_mode))
                                                                                              ,QRectF(curOffset,DEFAULT_ITEM_WIDTH,secondHeaderWidth(),DEFAULT_ITEM_WIDTH)
                                                                                              ,p_topHeaderItem);

                if(m_mode == days1)
                {
                    Qt::DayOfWeek d = (Qt::DayOfWeek)dt.date().dayOfWeek();
                    if (((d == Qt::Saturday) || (d == Qt::Sunday)))
                        p_secondHeaderItem->setTextColor( QColor(230,0,0) );
                }

                m_items.append(p_secondHeaderItem);
                curOffset+=secondHeaderWidth();

                dt = nextSecondHeaderDt(dt,m_mode);
            }

            p_topHeaderItem->setRect(QRectF(topHeaderStartsAt,0,curOffset-topHeaderStartsAt,DEFAULT_ITEM_WIDTH));

            m_items.append(p_topHeaderItem);
        }

        m_scene->updateWidth(curOffset);
    }
    else if(m_headerMode == TimelineMode)
    {
        calculateTimeMode();
        initRange();


    }
}

GanttHeader::GanttPrecisionMode GanttHeader::mode() const
{
    return m_mode;
}

void GanttHeader::setMode(const GanttPrecisionMode &mode)
{
    if(m_mode == mode)
        return;

    if(m_headerMode == TimelineMode)
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
    {
        updateHeader();

        updateWidget();
    }
}

void GanttHeader::onItemsAddition(GanttInfoItem* items)
{
    if(onItemsAdditionHelper(items))
    {
        updateHeader();

        updateWidget();
    }
}

void GanttHeader::initRange()
{
    static UtcDateTime lastMinDt = UtcDateTime(),
                        lastMaxDt = UtcDateTime();
    static GanttPrecisionMode lastMode = GanttPrecisionMode_count;

    if(lastMinDt != m_minDt
            || lastMaxDt != m_maxDt
            || lastMode != m_mode )
    {
        lastMinDt = m_minDt;
        lastMaxDt = m_maxDt;
        lastMode = m_mode;
    }
    else
        return;


    if(m_headerMode == GanttDiagramMode)
    {
        switch(m_mode)
        {
        case seconds1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),m_minDt.minute())));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(m_maxDt.hour(),m_maxDt.minute()).addSecs(SECONDS_IN_MINUTE)
                        ).addMicroseconds(-1);

            break;
        case minutes1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),0)));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(m_maxDt.hour(),0).addSecs(SECONDS_IN_HOUR)
                        ).addMicroseconds(-1);

            break;
        case hours1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day())));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day())).addDays(1)
                        ).addMicroseconds(-1);

            break;
        case days1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),1)));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),1)).addMonths(1)
                        ).addMicroseconds(-1);

            break;
        case months1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),1,1)));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),1,1)).addYears(1)
                        ).addMicroseconds(-1);

            break;
        default:
            break;
        }
    }
    else if(m_headerMode == TimelineMode)
    {
        switch(m_mode)
        {
        case seconds1:
            m_startDt.setDateTime(m_minDt.dateTime());
            m_finishDt = UtcDateTime(
                        m_maxDt.dateTime()
                        );
//            if(minEnd < m_maxDt.minute())
//                m_finishDt.addSecs(1);
            break;
        case seconds5:
        {
            int secBeg = m_minDt.second()/5 * 5,
                    secEnd = m_maxDt.second()/5 * 5;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),m_minDt.minute(),secBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_minDt.hour(),m_maxDt.minute(),secEnd)
                        );
            if(secEnd < m_maxDt.second())
                m_finishDt = m_finishDt.addSecs(5);
            break;
        }
        case seconds15:
        {
            int secBeg = m_minDt.second()/15 * 15,
                    secEnd = m_maxDt.second()/15 * 15;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),m_minDt.minute(),secBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_minDt.hour(),m_maxDt.minute(),secEnd)
                        );
            if(secEnd < m_maxDt.second())
                m_finishDt = m_finishDt.addSecs(15);
            break;
        }
        case seconds30:
        {
            int secBeg = m_minDt.second()/30 * 30,
                    secEnd = m_maxDt.second()/30 * 30;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),m_minDt.minute(),secBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_minDt.hour(),m_maxDt.minute(),secEnd)
                        );
            if(secEnd < m_maxDt.second())
                m_finishDt = m_finishDt.addSecs(30);
            break;
        }
        case minutes1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),m_minDt.minute())));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(m_maxDt.hour(),m_maxDt.minute())
                        );
            if(m_finishDt < m_maxDt)
                m_finishDt = m_finishDt.addSecs(SECONDS_IN_MINUTE);
            break;
        case minutes5:
        {
            int minBeg = m_minDt.minute()/5 * 5,
                    minEnd = m_maxDt.minute()/5 * 5;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),minBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_maxDt.hour(),minEnd)
                        );
            if(minEnd < m_maxDt.minute())
                m_finishDt = m_finishDt.addSecs(5*SECONDS_IN_MINUTE);
            break;
        }
        case minutes15:
        {
            int minBeg = m_minDt.minute()/15 * 15,
                    minEnd = m_maxDt.minute()/15 * 15;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),minBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_maxDt.hour(),minEnd)
                        );
            if(minEnd < m_maxDt.minute())
                m_finishDt = m_finishDt.addSecs(15*SECONDS_IN_MINUTE);
            break;
        }
        case minutes30:
        {
            int minBeg = m_minDt.minute()/30 * 30,
                    minEnd = m_maxDt.minute()/30 * 30;

            m_startDt.setDateTime(QDateTime(m_minDt.date(),QTime(m_minDt.hour(),minBeg)));
            m_finishDt = UtcDateTime(
                        m_maxDt.date(),QTime(m_maxDt.hour(),minEnd)
                        );
            if(minEnd < m_maxDt.minute())
                m_finishDt = m_finishDt.addSecs(30*SECONDS_IN_MINUTE);
            break;
        }
        case hours1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(m_minDt.hour(),0)));

            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(m_maxDt.hour(),0)
                        );


            if(m_finishDt < m_maxDt)
                m_finishDt = m_finishDt.addSecs(SECONDS_IN_HOUR);

            break;
        case hours6:
        {
            int hourBeg = m_minDt.hour() / 6 * 6,
                    hourEnd = m_maxDt.hour() / 6 * 6;

            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(hourBeg,0)));

            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(hourEnd,0)
                        );


            if(hourEnd < m_maxDt.hour())
                m_finishDt = m_finishDt.addSecs(modeToSecond(m_mode));

            break;
        }
        case hours12:
        {
            int hourBeg = m_minDt.hour() / 12 * 12,
                    hourEnd = m_maxDt.hour() / 12 * 12;

            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),m_minDt.day()),QTime(hourBeg,0)));

            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),m_maxDt.day()))
                        ,QTime(hourEnd,0)
                        );


            if(hourEnd < m_maxDt.hour())
                m_finishDt = m_finishDt.addSecs(modeToSecond(m_mode));

            break;
        }
        case days1:
            m_startDt.setDateTime(QDateTime(m_minDt.date()));
            m_finishDt = UtcDateTime(
                        m_maxDt.date()
                        );
            if(m_finishDt < m_maxDt)
                m_finishDt = m_finishDt.addDays(1);
            break;
        case months1:
            m_startDt.setDateTime(QDateTime(QDate(m_minDt.year(),m_minDt.month(),1)));
            m_finishDt = UtcDateTime(
                        (QDate(m_maxDt.year(),m_maxDt.month(),1))
                        );
            if(m_finishDt < m_maxDt)
                m_finishDt = m_finishDt.addMonths(1);
            break;
        default:
            qDebug() <<"GanttHeader::initRange() out of range";
            break;
        }

    }

    setLengthInMicroseconds(m_startDt.microsecondsTo(m_finishDt));

    if(m_widget)
        m_widget->repaintDtHeader();

//    qDebug()  << "m_lengthInMicroseconds = " << QString::number(m_lengthInMicroseconds);
}

QString GanttHeader::topHeaderFormat(GanttHeader::GanttPrecisionMode mode)
{
    switch (mode)
    {
    case seconds1:
        return ("ddd d MMM yy, hh:mm");
        break;
    case minutes1:
        return ("ddd d MMM yy, hh:00");
        break;
    case hours1:
        return ("ddd d MMM yy");
        break;
    case days1:
        return ("MMM yyyy");
        break;
    case months1:
        return ("yyyy");
        break;
    default:
        return QString();
    }
}

QString GanttHeader::secondHeaderFormat(GanttHeader::GanttPrecisionMode mode)
{
    switch (mode)
    {
    case seconds1:
        return ("s");
        break;
    case minutes1:
        return ("m");
        break;
    case hours1:
        return ("h");
        break;
    case days1:
        return ("d");
        break;
    case months1:
        return ("MMM");
        break;
    default:
        return QString();
    }
}

UtcDateTime GanttHeader::nextTopHeaderDt(const UtcDateTime &curDt,GanttHeader::GanttPrecisionMode mode)
{
    int i_mode = mode;
    if(i_mode)
        i_mode--;
    return curDt.addMicroseconds(
                modeToMicrosecond(static_cast<GanttPrecisionMode>(i_mode),curDt.date()));
}

UtcDateTime GanttHeader::nextSecondHeaderDt(const UtcDateTime &curDt,GanttHeader::GanttPrecisionMode mode)
{
    return curDt.addMicroseconds(modeToMicrosecond(mode,curDt.date()));
}

qreal GanttHeader::secondHeaderWidth() const
{
    if(m_mode == months1)
        return DEFAULT_MONTH_WIDTH;

    return DEFAULT_ITEM_WIDTH;
}

void GanttHeader::clear()
{
    foreach(QGraphicsRectItemWithText* item, m_items)
    {
        item->setParentItem(NULL);
        m_scene->removeItem(item);
        m_items.removeOne(item);
        delete item;
    }
    m_scene->invalidate();
}

qreal GanttHeader::dtToX(const UtcDateTime &dt) const
{
    qreal sceneW = m_scene->width();

    return
            ((m_headerMode==TimelineMode)?(MIN_WIDTH_FOR_TIME_VISUALIZING/2):(0)) +
           ((1.0)*startDt().microsecondsTo(dt))/m_lengthInMicroseconds *
            (sceneW-((m_headerMode==TimelineMode)?(MIN_WIDTH_FOR_TIME_VISUALIZING):(0)));

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

        newRange |= verifyBoundsByLeaf(leaf);
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

    setLengthInMicroseconds(m_minDt.microsecondsTo(m_maxDt));

    return newRange;
}

QString GanttHeader::formatForMode(GanttHeader::GanttPrecisionMode mode)
{
    switch(mode)
    {
    case seconds1:
    case seconds5:
    case seconds15:
    case seconds30:
        return "mm:ss";
    case minutes1:
    case minutes5:
    case minutes15:
    case minutes30:
        return "hh:mm";
    case hours1:
    case hours6:
    case hours12:
        return "hh:00";

    case days1:
        return "dd";
    case months1:
        return "MMM";
    default:
        qDebug() << "GanttHeader::formatForMode out of range";
        return QString();
    }
}

QString GanttHeader::textForDtStep(int step) const
{
    switch(m_mode)
    {
    case seconds1:
    case seconds5:
    case seconds15:
    case seconds30:
        return m_startDt.addSecs( modeToSecond(m_mode) * step).toString("mm:ss");
    case minutes1:
    case minutes5:
    case minutes15:
    case minutes30:
        return m_startDt.addSecs(modeToSecond(m_mode) * step).toString("hh:mm");
    case hours1:
    case hours6:
    case hours12:
        return m_startDt.addSecs(modeToSecond(m_mode) * step).toString("hh:00");

    case days1:
        return m_startDt.addDays(step).toString("d");
    case months1:
        return m_startDt.addMonths(step).toString("MMM");
    default:
        qDebug() <<"GanttHeader::textForDtStep out of range";
       return QString();
    }
    return QString();
}

void GanttHeader::updateWidget()
{
    if(!m_widget)
        return;

    m_widget->updateRange();
}

void GanttHeader::setLengthInMicroseconds(long long lengthInMicroseconds)
{
    m_lengthInMicroseconds = lengthInMicroseconds;

}

const UtcDateTime &GanttHeader::startDt() const
{
//    if(m_headerMode == TimelineMode)
//        return m_minDt;
//    if(m_headerMode == GanttDiagramMode)
        return m_startDt;

}

const UtcDateTime &GanttHeader::finishDt() const
{
//    if(m_headerMode == TimelineMode)
//        return m_maxDt;
//    if(m_headerMode == GanttDiagramMode)
        return m_finishDt;

}

void GanttHeader::setRange(UtcDateTime min, UtcDateTime max)
{
    if(min>max)
        return;

    qDebug() << "GanttHeader::setRange "
             << min <<' ' <<max;

    m_minDt = min;
    m_maxDt = max;

    updateHeader();
}

bool GanttHeader::verifyBoundsByLeaf(const GanttInfoLeaf *leaf)
{
    if(!leaf)
        return false;
    if(!leaf->start().isValid() || !leaf->finish().isValid())
    {
        qDebug()<< "Leaf\'s time not valid!";
        return false;
    }

    bool newRange = false;
    if(m_minDt > leaf->start())
    {
        m_minDt = leaf->start();
        newRange = true;
    }

    if(m_maxDt < leaf->finish())
    {
        m_maxDt = leaf->finish();
        newRange = true;
    }

    return newRange;
}

long long GanttHeader::modeToMicrosecond(GanttPrecisionMode mode, const QDate& date)
{

    switch(mode)
    {
//    case microseconds1:
//        return 1;
//    case microseconds15:
//        return 15;
//    case microseconds30:
//        return 30;
//    case miliseconds1:
//        return _MICROSECONDS_IN_MILISECOND;
//    case miliseconds15:
//        return 15*_MICROSECONDS_IN_MILISECOND;
//    case miliseconds30:
//        return 30*_MICROSECONDS_IN_MILISECOND;
    case seconds1:
        return _MICROSECONDS_IN_SECOND;
    case seconds5:
        return 5*_MICROSECONDS_IN_SECOND;
    case seconds15:
        return 15*_MICROSECONDS_IN_SECOND;
    case seconds30:
        return 30*_MICROSECONDS_IN_SECOND;
    case minutes1:
        return SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes5:
        return 5*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes15:
        return 15*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes30:
        return 30*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case hours1:
        return ((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case hours6:
        return 6*((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case hours12:
        return 12*((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case days1:
        return ((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
    case months1:
        if(!date.isValid())
            return 28*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
        return date.daysInMonth()*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
    default:
        qDebug() <<"long long GanttHeader::modeToMicrosecond(GanttHeader::GanttHeaderMode mode) out of range";
        return 0;
    }
}

long long GanttHeader::modeToSecond(GanttHeader::GanttPrecisionMode mode, const QDate& date)
{
    switch(mode)
    {
    case seconds1:
        return 1;
    case seconds5:
        return 5;
    case seconds15:
        return 15;
    case seconds30:
        return 30;
    case minutes1:
        return SECONDS_IN_MINUTE;
    case minutes5:
        return 5*SECONDS_IN_MINUTE;
    case minutes15:
        return 15*SECONDS_IN_MINUTE;
    case minutes30:
        return 30*SECONDS_IN_MINUTE;
    case hours1:
        return ((long long)SECONDS_IN_HOUR);
    case hours6:
        return 6*((long long)SECONDS_IN_HOUR);
    case hours12:
        return 12*((long long)SECONDS_IN_HOUR);
    case days1:
        return ((long long)SECONDS_IN_DAY);
    case months1:
        if(!date.isValid())
            return 28*((long long)SECONDS_IN_DAY);
        return date.daysInMonth()*((long long)SECONDS_IN_DAY);
    default:
        qDebug() <<"GanttHeader::modeToSecond out of range";
        return 0;
    }
}

int GanttHeader::modeToSegmentCount(GanttHeader::GanttPrecisionMode mode, const QDate& date)
{
    switch(mode)
    {
    case seconds1:
        return 1;
    case seconds5:
        return 5;
    case seconds15:
        return 3;
    case seconds30:
        return 6;
    case minutes1:
        return 6;
    case minutes5:
        return 5;
    case minutes15:
        return 3;
    case minutes30:
        return 6;
    case hours1:
        return 6;
    case hours6:
        return 6;
    case hours12:
        return 6;
    case days1:
        return 6;
    case months1:
        if(!date.isValid())
            return 6;
        return date.daysInMonth();
    default:
        qDebug() <<"GanttHeader::modeToSecond out of range";
        return 0;
    }
}

bool GanttHeader::isDrawn(const UtcDateTime &dt, GanttHeader::GanttPrecisionMode mode)
{
    if(!dt.isValid())
    {
        qDebug() << "GanttHeader::isDrawn dt is not valid";
        return false;
    }

    switch(mode)
    {
    case seconds1:
        return true;
    case seconds5:
        return !(dt.second()%5);
    case seconds15:
        return !(dt.second()%15);
    case seconds30:
        return !(dt.second()%30);
    case minutes1:
        return !(dt.second());
    case minutes5:
        return !(dt.minute()%5);
    case minutes15:
        return !(dt.minute()%15);
    case minutes30:
        return !(dt.minute()%30);
    case hours1:
        return !(dt.minute());
    case hours6:
        return !(dt.hour()%6);
    case hours12:
        return !(dt.hour()%12);
    case days1:
        return !(dt.hour());
    case months1:
        return !(dt.day() - 1);
    default:
        qDebug() <<"GanttHeader::isDrawn out of range";
        return false;
    }
}

void GanttHeader::setCurrentWidth(int currentWidth)
{
    if(m_currentWidth == currentWidth)
        return;
    m_currentWidth = currentWidth;

    updateHeader();
}

GanttHeader::GanttHeaderMode GanttHeader::headerMode() const
{
    return m_headerMode;
}

void GanttHeader::calculateTimeMode()
{
    if(m_isEmpty)
        return;
    if(m_currentWidth < MIN_WIDTH_FOR_TIME_VISUALIZING)
        return;

    qDebug() << "min: "<<m_minDt;
    qDebug() << "max: "<<m_maxDt;


    long long coef = (m_minDt.microsecondsTo(m_maxDt)*1.0/((m_currentWidth-MIN_WIDTH_FOR_TIME_VISUALIZING)/MIN_WIDTH_FOR_TIME_VISUALIZING));

    GanttPrecisionMode mode = GanttPrecisionMode_count;

    for(int i = GanttPrecisionMode_count-1; i >= 0; --i)
    {
        GanttPrecisionMode i_mode = static_cast<GanttPrecisionMode>(i);
        if(coef < modeToMicrosecond(i_mode))
        {
            mode = i_mode;
            break;
        }
    }

    if(mode == GanttPrecisionMode_count)
    {
        qDebug() << QString::number(coef);
        qDebug() << QString::number(modeToMicrosecond(months1));


        qDebug() << "GanttHeader::calculateTimeMode() out of range";
        return;
    }

    if(m_mode != mode)
    {
        qDebug() << "mode changed " <<QString::number(mode);
        m_mode = mode;
        initRange();
    }

    qreal newCoef = (m_startDt.microsecondsTo(m_finishDt)*1.0/((m_currentWidth-MIN_WIDTH_FOR_TIME_VISUALIZING)/MIN_WIDTH_FOR_TIME_VISUALIZING));

    if(newCoef> modeToMicrosecond(m_mode))
    {
        qDebug() << "newCoeff doesn\'t fit";
        m_mode = (mode)?((GanttPrecisionMode)((int)mode - 1)):((GanttPrecisionMode)0);
        initRange();
        newCoef = (m_startDt.microsecondsTo(m_finishDt)*1.0/((m_currentWidth-MIN_WIDTH_FOR_TIME_VISUALIZING)/MIN_WIDTH_FOR_TIME_VISUALIZING));

    }


    m_stretchFactor = modeToMicrosecond(m_mode) * 1.0 / newCoef;
}

bool GanttHeader::setHeaderMode(const GanttHeaderMode &headerMode)
{
    if(headerMode == m_headerMode)
        return false;
    m_headerMode = headerMode;

    if(!m_scene)
        return false;

    GanttGraphicsView *p_view;

    if(!m_scene->views().isEmpty())
        p_view = dynamic_cast<GanttGraphicsView*>(m_scene->views()[0]);

    if(headerMode == TimelineMode)
    {
        clear();
    }
    else if(headerMode == GanttDiagramMode)
    {
        if(m_mode == seconds5)
            setMode(seconds1);
        if (m_mode == seconds15 || m_mode == seconds30 || m_mode == minutes5)
            setMode(minutes1);
        else if(m_mode == minutes15 || m_mode == minutes30)
            setMode(hours1);
        else if (m_mode == hours6 || m_mode == hours12)
            setMode(days1);
    }

    init();

    updateHeader();

    return true;
}

void GanttHeader::init()
{
    if(!m_scene)
        return;

    GanttGraphicsView *p_view = NULL;

    if(!m_scene->views().isEmpty())
        p_view = dynamic_cast<GanttGraphicsView*>(m_scene->views()[0]);

    if(!p_view)
        return;

    if(m_headerMode == GanttDiagramMode)
    {
        p_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else if(m_headerMode == TimelineMode)
    {
        p_view->setMinimumWidth(GANTTGRAPHICSVIEW_MIN_WIDTH);
        p_view->setMaximumWidth(100000);
        p_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

