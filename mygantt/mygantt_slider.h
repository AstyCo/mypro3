#ifndef GANTTSLIDER_H
#define GANTTSLIDER_H

#include "utcdatetime.h"


#include <QGraphicsObject>

class GanttHeader;

class GanttSlider : public QGraphicsObject
{
    Q_OBJECT

public:
    GanttSlider(QGraphicsItem * parent = 0);

    void setScene(QGraphicsScene* scene);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void updateShape();
    qreal sliderPos() const;
    void setSlidersRect(const QRectF &slidersRect);

    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);



signals:
    void sliderPosChanged(qreal sliderPos);

public slots:
    void setSliderPos(const qreal &sliderPos);

private:
    void updatePos();

private:
    qreal m_sliderPos; ///< 0 - левый край, 1 - правый край
    QRectF m_slidersRect; ///< Область сцены, в которой слайдер может перемещаться

    QPainterPath m_sliderShape;
    QPainterPath m_rhombus;

    qreal m_penWidth;

};

#endif // GANTTSLIDER_H
