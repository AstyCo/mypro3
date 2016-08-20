#ifndef GANTTINTERVALSLIDER_H
#define GANTTINTERVALSLIDER_H

#include "intervalslider.h"
#include "utcdatetime.h"

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


public slots:
    void setCurrentTimePos(const qreal &currentTimePos);

protected:
    void drawHandle(QPainter *painter, const QRect &handleRect, bool is_selected) const;
    void drawSliderLine(QPainter *painter, const QRect &sliderRect) const;

    void mouseMoveEvent(QMouseEvent *e);
    bool moveHandles(long long deltaVal);
    void mousePressEvent(QMouseEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    qreal m_currentTimePos;

    qreal m_currentTimeRectWidth;
    QColor m_currentTimeRectColor;

    GanttScene* m_scene;
    GanttWidget* m_widget;
    long long m_shiftRange;
    QPoint m_lastPos;
};

#endif // GANTTSLIDER_H
