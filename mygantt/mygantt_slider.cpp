#include "mygantt_slider.h"
#include "mygantt_globalvalues.h"


#include <QPainter>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

GanttSlider::GanttSlider(QGraphicsItem* parent) :
    QGraphicsObject(parent)
{
    m_penWidth = 2;
    setCursor(Qt::OpenHandCursor);
    setZValue(20);
}

void GanttSlider::setScene(QGraphicsScene *scene)
{
    if(!scene)
        return;

    setSlidersRect(scene->sceneRect());
    scene->addItem(this);
}

QRectF GanttSlider::boundingRect() const
{
    return m_sliderShape.controlPointRect();
}

void GanttSlider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush borderBrush(QColor(Qt::black).lighter(130));
    QBrush fillBrush(QColor(Qt::red));

    QPen pen(QBrush(QColor(Qt::black)), 1 ,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin);

    painter->fillPath(m_sliderShape,borderBrush);
    painter->fillPath(m_rhombus,fillBrush);

}

QPainterPath GanttSlider::shape() const
{
    return m_sliderShape;
}

void GanttSlider::updateShape()
{
    QPainterPath path;
    qreal top = 0,
            bottom = m_slidersRect.height(),
            x = 0;

    path.moveTo(x - m_penWidth/2,top);
    path.lineTo(x + m_penWidth/2,top);
    path.lineTo(x + m_penWidth/2,bottom);
    path.lineTo(x - m_penWidth/2,bottom);
    path.lineTo(x - m_penWidth/2,top);

    qreal diagonal = (3.0/4)*DEFAULT_ITEM_HEIGHT;


    QPainterPath rhombus;
    rhombus.moveTo(QPointF(x - diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));
    rhombus.lineTo(QPointF(x ,DEFAULT_HEADER_HEIGHT - diagonal));
    rhombus.lineTo(QPointF(x + diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));
    rhombus.lineTo(QPointF(x ,DEFAULT_HEADER_HEIGHT));
    rhombus.lineTo(QPointF(x - diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));

    path.addPath(rhombus);

    prepareGeometryChange();
    m_rhombus = rhombus;
    m_sliderShape = path;
}


QVariant GanttSlider::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange && scene())
    {
        qreal newHPos = value.toPointF().x();

        if(newHPos < m_slidersRect.left())
        {
            newHPos = m_slidersRect.left();
        }
        if(newHPos > m_slidersRect.right())
        {
            newHPos = m_slidersRect.right();
        }

        if(m_slidersRect.width())
            setSliderPos((newHPos-m_slidersRect.left())/(m_slidersRect.width()));

        return QGraphicsItem::itemChange(change,value);
    }
    return QGraphicsItem::itemChange(change,value);
}

void GanttSlider::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!scene())
        return;

    if(cursor().shape() == Qt::ClosedHandCursor)
    {
        QPointF eventScenePos = mapToScene(event->pos());

        setPos(eventScenePos.x(),scenePos().y());
    }
}

void GanttSlider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
}

void GanttSlider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

qreal GanttSlider::sliderPos() const
{
    return m_sliderPos;
}

void GanttSlider::setSliderPos(const qreal &sliderPos)
{
    if(sliderPos == m_sliderPos)
        return;
    m_sliderPos = sliderPos;

    emit sliderPosChanged(sliderPos);
}

void GanttSlider::updatePos()
{
    setPos(m_slidersRect.x() + m_sliderPos*m_slidersRect.width(), m_slidersRect.top());
}

void GanttSlider::setSlidersRect(const QRectF &slidersRect)
{
    m_slidersRect = slidersRect;
    updateShape();

    updatePos();

}

void GanttSlider::setPos(const QPointF &pos)
{
    qreal x = pos.x();

    if(x < m_slidersRect.left())
    {

        x = m_slidersRect.left();
    }
    if(x > m_slidersRect.right())
    {
        x = m_slidersRect.right();
    }

    if(m_slidersRect.width())
        setSliderPos((x-m_slidersRect.left())/(m_slidersRect.width()));

    QGraphicsItem::setPos(x,pos.y());

    if(scene())
        scene()->invalidate(QRectF(),QGraphicsScene::BackgroundLayer);
}

void GanttSlider::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}


