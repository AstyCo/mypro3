#ifndef GANTTINTERVALSLIDER_H
#define GANTTINTERVALSLIDER_H

#include "intervalslider.h"
#include "utcdatetime.h"

#include "mygantt_header.h"

class GanttScene;
class GanttWidget;

class GanttIntervalSlider : public IntervalSlider
{
    Q_OBJECT

public:
    explicit GanttIntervalSlider(QWidget *parent = 0);


    void setCurrentTimeRectWidth(const qreal &currentTimeRectWidth);
    void setCurrentTimeRectColor(const QColor &currentTimeRectColor);

    void setScene(GanttScene *scene);

    UtcDateTime posToDt(qreal pos) const;
    UtcDateTime valToDt(long long val) const;
    long long dtToVal(const UtcDateTime& dt) const;
    UtcDateTime closestStartDt(long long val) const;
    UtcDateTime closestFinishDt(long long val) const;

    UtcDateTime beginDt() const;
    UtcDateTime endDt() const;

    void setLimits(long long minValue, long long maxValue);


    long long minTimeSize() const;

    void setBeginHandle(long long beginHandle);
    void setEndHandle(long long endHandle);

private:
    UtcDateTime closestStartDtHelper(const UtcDateTime& val, GanttHeader::GanttPrecisionMode mode) const;
    UtcDateTime closestFinishDtHelper(const UtcDateTime& val, GanttHeader::GanttPrecisionMode mode) const;


public slots:
    void setCurrentTimePos(const qreal &currentTimePos);
    void setCurrentTime(const UtcDateTime &dt);
    void checkLimits(const UtcDateTime &start, const UtcDateTime &finish);
    void updateRange();
    void setMinTimeSize(long long minTimeSize);
    void updateMinTimeSize(const QSize& newViewSize);

protected:
    void drawHandle(QPainter *painter, const QRect &handleRect, bool is_selected) const;
    void drawSliderLine(QPainter *painter, const QRect &sliderRect) const;

    void mouseMoveEvent(QMouseEvent *e);
    bool moveHandles(long long deltaVal);
    void mousePressEvent(QMouseEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    UtcDateTime m_currentTime;
    qreal m_currentTimePos;

    qreal m_currentTimeRectWidth;
    QColor m_currentTimeRectColor;

    GanttScene* m_scene;
    GanttWidget* m_widget;
    long long m_shiftRange;
    long long m_minTimeSize;
    QPoint m_lastPos;
};

#endif // GANTTSLIDER_H
