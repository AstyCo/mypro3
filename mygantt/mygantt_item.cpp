#include "mygantt_item.h"
#include "mygantt_scene.h"

#include <QPainter>

GanttItem::GanttItem(GanttInfoLeaf *info,QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    m_scene = NULL;
    m_info = info;
}

QRectF GanttItem::boundingRect() const
{
    return QRectF(QPointF(0,0),m_boundingRectSize);
}

void GanttItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);



    painter->fillRect(boundingRect().adjusted(0,5,0,-5),QBrush(m_info->getColor()));
    painter->drawRect(boundingRect().adjusted(0,5,0,-5));
}

void GanttItem::setScene(GanttScene *scene)
{
    m_scene = scene;
    m_scene->addItem(this);
}
void GanttItem::setBoundingRectSize(const QSizeF &boundingRectSize)
{
    prepareGeometryChange();
    m_boundingRectSize = boundingRectSize;
}
GanttInfoLeaf *GanttItem::info() const
{
    return m_info;
}

QRectF GanttItem::rect() const
{
    return QRectF(pos(),m_boundingRectSize);
}



