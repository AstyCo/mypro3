#include "mygantt_intervalslider.h"
#include "mygantt_scene.h"
#include "mygantt_widget.h"

#include "utcdatetime.h"

#include <QPainter>
#include <QLinearGradient>
#include <QBrush>
#include <QFrame>

#include <QMouseEvent>

#include <QDebug>

GanttIntervalSlider::GanttIntervalSlider(QWidget *parent )
    : IntervalSlider(parent)
{
    setOffsetV(2);
    setSliderV(7);
    setHandleSize(5);

    setCurrentTimeRectWidth(3);
    setCurrentTimePos(0);
    setCurrentTimeRectColor(Qt::red);

    setStyleSheet("background-color: rgb(21, 168, 194)");

    m_shiftRange = 0;
    m_widget = NULL;
    QWidget *p_parent = parentWidget();
    while(!m_widget)
    {
        if(!p_parent)
            break;

        m_widget = dynamic_cast<GanttWidget*>(p_parent);
        p_parent = p_parent->parentWidget();
    }

}

void GanttIntervalSlider::drawHandle(QPainter *painter, const QRect &handleRect, bool is_selected) const
{
//    return IntervalSlider::drawHandle(painter,handleRect,is_selected);

    painter->setPen(Qt::black);
    painter->drawRect(handleRect);

    painter->fillRect(handleRect,QBrush((is_selected)?(Qt::black):(QColor(Qt::blue))));
}

void GanttIntervalSlider::drawSliderLine(QPainter *painter, const QRect &sliderRect) const
{
//    return IntervalSlider::drawSliderLine(painter,sliderRect);

    int top = sliderRect.y() + m_offsetV,
            width = sliderRect.width() - 2 * halfHandleSize();

    int beginRectLeft = sliderRect.x() + halfHandleSize(),
            beginRectWidth = valueToPoint(m_beginValue,BeginHandle) - halfHandleSize() - sliderRect.x(),
            innerRectLeft = beginRectLeft + beginRectWidth,
            innerRectWidth = valueToPoint(m_endValue,EndHandle) - beginRectWidth,
            endRectLeft = innerRectLeft + innerRectWidth,
            endRectWidth = width - valueToPoint(m_endValue,EndHandle);
    QLinearGradient foregroundGradient(QPoint(0,top),QPoint(0,top + m_sliderV));
    foregroundGradient.setColorAt(0,Qt::cyan);
    foregroundGradient.setColorAt(0.5,QColor(Qt::cyan).lighter(180));
    foregroundGradient.setColorAt(1,Qt::cyan);

    QLinearGradient backgroundGradient(QPoint(0,top),QPoint(0,top + m_sliderV));
    backgroundGradient.setColorAt(0,QColor(Qt::cyan));
    backgroundGradient.setColorAt(0.5,Qt::white);
    backgroundGradient.setColorAt(1,QColor(Qt::cyan));

    painter->setPen(Qt::gray);
    painter->drawRect(QRect(beginRectLeft,
                            top,
                            width,
                            m_sliderV
                            ));

    painter->fillRect(QRect(beginRectLeft,
                            top,
                            width,
                            m_sliderV
                            ), QBrush(QColor(Qt::cyan).lighter(190)));

    if(beginRectWidth > 0)
    {
        QRect beforeBeginRect(beginRectLeft,
                              top,
                              beginRectWidth,
                              m_sliderV);

        painter->fillRect(beforeBeginRect,foregroundGradient);

    }


    if(endRectWidth > 0)
    {
        QRect afterEndRect(endRectLeft,
                        top,
                        endRectWidth,
                        m_sliderV);

        painter->fillRect(afterEndRect,foregroundGradient);
    }

    // DRAW CURRENT TIME
    QRect currentTimeRect(m_currentTimePos * width - m_currentTimeRectWidth/2 + halfHandleSize(),
                          sliderRect.y(),
                          m_currentTimeRectWidth,
                          intervalSliderHeight()
                          );
    painter->drawRect(currentTimeRect);
    painter->fillRect(currentTimeRect, m_currentTimeRectColor);

}

void GanttIntervalSlider::mouseMoveEvent(QMouseEvent *e)
{
    long long val;

    if(m_clippedHandle==BeginHandle)
    {
        val = pointToValue(e->pos(),BeginHandle);
        if(m_shiftModifier)
        {
            moveHandles(0);
        }
        else if(m_scene->startByDt(valToDt(beginHandle())) != closestStartDt(val))
        {
            setBeginHandle(dtToVal(closestStartDt(val)));
        }
    }
    else if(m_clippedHandle==EndHandle)
    {
        val = pointToValue(e->pos(),EndHandle);
        if(m_shiftModifier)
        {
            moveHandles(0);
        }
        else if(m_scene->finishByDt(valToDt(endHandle())) != closestFinishDt(val))
            {
                setEndHandle(dtToVal(m_scene->finishByDt(closestFinishDt(val))));
            }
    }
    m_lastPos = e->pos();
}

bool GanttIntervalSlider::moveHandles(long long deltaVal)
{
    Q_UNUSED(deltaVal);

    if(m_scene)
    {
        UtcDateTime lastStart =  m_scene->startByDt(valToDt(beginHandle())),
                       lastFinish = m_scene->finishByDt(valToDt(endHandle()));

        UtcDateTime newStart,
                 newFinish;

        long long beg,end;
        if(m_clippedHandle == BeginHandle)
        {
            beg = pointToValue(m_lastPos, BeginHandle);
            end = beg + m_shiftRange;
        }
        else if (m_clippedHandle == EndHandle)
        {
            end = pointToValue(m_lastPos, EndHandle);
            beg = end - m_shiftRange;
        }

        if(beg < m_minValue)
        {

            beg = m_minValue;
            end = beg + m_shiftRange;
        }
        if(end > m_maxValue)
        {
            end = m_maxValue;
            beg = end - m_shiftRange;
        }

        newStart = closestStartDt(beg);
        newFinish = closestFinishDt(end);

        bool changedStart = false, changedFinish = false;

        blockSignals(true);
        if(newStart != lastStart)
        {
            setBeginHandle(dtToVal(newStart));
            changedStart = true;
        }
        if(newFinish != lastFinish)
        {
            setEndHandle(dtToVal(newFinish));
            changedFinish = true;
        }
        blockSignals(false);

        if(changedStart)
        {
            emit valueChanged(IntervalSlider::BeginHandle,m_beginValue);
            emit beginMoved(m_beginValue);
        }
        if(changedFinish)
        {
            emit valueChanged(IntervalSlider::EndHandle,m_endValue);
            emit endMoved(m_endValue);
        }
        return (m_clippedHandle == BeginHandle && changedStart
                || m_clippedHandle == EndHandle && changedFinish);
    }
    return false;
}

void GanttIntervalSlider::mousePressEvent(QMouseEvent *e)
{
    m_lastPos = e->pos();
    IntervalSlider::mousePressEvent(e);
}

void GanttIntervalSlider::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Shift)
    {
        m_shiftRange = endHandle() - beginHandle();
    }

    IntervalSlider::keyPressEvent(e);
}

void GanttIntervalSlider::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Shift)
    {
        m_shiftRange = 0;
    }

    IntervalSlider::keyReleaseEvent(e);
}

void GanttIntervalSlider::setScene(GanttScene *scene)
{
    m_scene = scene;
}

UtcDateTime GanttIntervalSlider::posToDt(qreal pos) const
{
    if(!m_widget)
        return UtcDateTime();
    return m_widget->minDt().addMicroseconds(pos * (m_widget->minDt().microsecondsTo(m_widget->maxDt())) + 0.5);
}

UtcDateTime GanttIntervalSlider::valToDt(long long val) const
{
    return UtcDateTime::fromMicrosecondsSinceEpoch(val);
}

long long GanttIntervalSlider::dtToVal(const UtcDateTime &dt) const
{
    return dt.toMicrosecondsSinceEpoch();
}

UtcDateTime GanttIntervalSlider::closestStartDt(long long val) const
{
    if(!m_scene)
    {
        qDebug() << "GanttIntervalSlider::closestStartDt m_scene is NULL";
        return UtcDateTime();

    }
    UtcDateTime valDt = valToDt(val);
    UtcDateTime start = m_scene->startByDt(valDt),
                nextStart = m_scene->nextStart(start);

    if(valDt.toMicrosecondsSinceEpoch() - start.toMicrosecondsSinceEpoch()
            > nextStart.toMicrosecondsSinceEpoch() - valDt.toMicrosecondsSinceEpoch() )
        return nextStart;
    return start;
}

UtcDateTime GanttIntervalSlider::closestFinishDt(long long val) const
{
    if(!m_scene)
    {
        qDebug() << "GanttIntervalSlider::closestFinishDt m_scene is NULL";
        return UtcDateTime();

    }
    UtcDateTime valDt = valToDt(val);
    UtcDateTime finish = m_scene->finishByDt(valDt),
                prevFinish = m_scene->prevFinish(finish);

    if(valDt.toMicrosecondsSinceEpoch() - prevFinish.toMicrosecondsSinceEpoch()
            > finish.toMicrosecondsSinceEpoch() - valDt.toMicrosecondsSinceEpoch() )
        return finish;
    return prevFinish;
}

void GanttIntervalSlider::setCurrentTimeRectColor(const QColor &currentTimeRectColor)
{
    m_currentTimeRectColor = currentTimeRectColor;

    repaint();
}

void GanttIntervalSlider::setCurrentTimePos(const qreal &currentTimePos)
{
    m_currentTimePos = currentTimePos;

    repaint();
}


void GanttIntervalSlider::setCurrentTimeRectWidth(const qreal &currentTimeRectWidth)
{
    m_currentTimeRectWidth = currentTimeRectWidth;

    repaint();
}

