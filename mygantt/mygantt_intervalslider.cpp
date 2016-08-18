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
            width = sliderRect.width() - 2 * halfHandleH();

    int beginRectLeft = sliderRect.x() + halfHandleH(),
            beginRectWidth = valueToPoint(m_beginValue,BeginHandle) - halfHandleH() - sliderRect.x(),
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
    QRect currentTimeRect(m_currentTimePos * width - m_currentTimeRectWidth/2 + halfHandleH(),
                          sliderRect.y(),
                          m_currentTimeRectWidth,
                          intervalSliderHeight()
                          );
    painter->drawRect(currentTimeRect);
    painter->fillRect(currentTimeRect, m_currentTimeRectColor);

}

void GanttIntervalSlider::mouseMoveEvent(QMouseEvent *e)
{
    long long val,deltaVal;

    if(m_clippedHandle==BeginHandle)
    {
        val = pointToValue(e->pos(),BeginHandle);
        if(m_shiftModifier)
        {
            deltaVal = val-beginHandle();
            if(beginHandle() + m_shiftRange + deltaVal>m_maxValue)
            {
                deltaVal=m_maxValue-(beginHandle() + m_shiftRange);
            }
            moveHandles(deltaVal);
        }
        else if(m_scene->startByDt(valToDt(beginHandle())) != m_scene->startByDt(valToDt(val)))
        {
            setBeginHandle(dtToVal(m_scene->startByDt(valToDt(val))));
        }
    }
    else if(m_clippedHandle==EndHandle)
    {
        val = pointToValue(e->pos(),EndHandle);
        if(m_shiftModifier)
        {
            deltaVal = val-endHandle();
            if(beginHandle()+deltaVal<m_minValue)
            {
                deltaVal=-beginHandle()+m_minValue;
            }
            moveHandles(deltaVal);
        }
        else if(m_scene->finishByDt(valToDt(endHandle())) != m_scene->finishByDt(valToDt(val)))
            {
                setEndHandle(dtToVal(m_scene->finishByDt(valToDt(val))));
            }
    }
}

void GanttIntervalSlider::moveHandles(long long deltaVal)
{
    if(m_scene)
    {
        UtcDateTime lastStart =  m_scene->startByDt(valToDt(beginHandle())),
                       lastFinish = m_scene->finishByDt(valToDt(endHandle()));

        UtcDateTime newStart = m_scene->startByDt(valToDt(beginHandle()+deltaVal))
                , newFinish = m_scene->finishByDt(valToDt(beginHandle() + m_shiftRange + deltaVal));

        if(newStart != lastStart || newFinish != lastFinish)
        {
            setBeginHandle(dtToVal(newStart));
            setEndHandle(dtToVal(newFinish));

            emit beginMoved(beginHandle());
            emit valueChanged(BeginHandle , beginHandle());
            emit endMoved(endHandle());
            emit valueChanged(EndHandle , endHandle());
            return;
        }
        else
            return;
    }
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

