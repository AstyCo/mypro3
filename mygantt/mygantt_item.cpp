#include "mygantt_item.h"
#include "mygantt_scene.h"

#include "mygantt_header.h"
#include "mygantt_infonode.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GanttItem::GanttItem(GanttInfoLeaf *info,QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    m_scene = NULL;
    m_header = dynamic_cast<GanttHeader*>(parent);

    m_info = info;
    connect(m_info, SIGNAL(changed()),this,SLOT(updateGeometry()));
}

QRectF GanttItem::boundingRect() const
{
    return QRectF(QPointF(0,0),m_boundingRectSize);
}

void GanttItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF drawRect = boundingRect().adjusted(0,5,0,-5);

    painter->fillRect(drawRect,QBrush(m_info->getColor()));
    painter->drawRect(drawRect);
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

void GanttItem::updateGeometry()
{
    if(!m_header)
        return;

    qreal startPos = m_header->dtToX(m_info->start()),
          itemWidth = m_header->dtToX(m_info->finish()) - startPos;

    setBoundingRectSize(QSizeF(itemWidth, DEFAULT_ITEM_WIDTH));
    setPos(startPos, 2*DEFAULT_ITEM_WIDTH + m_info->pos());
}
GanttInfoLeaf *GanttItem::info() const
{
    return m_info;
}

QRectF GanttItem::rect() const
{
    return QRectF(pos(),m_boundingRectSize);
}

void GanttItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if(!m_scene || !m_info)
        return;

    if(m_info->parent())
        m_scene->changeExpanding(m_info->parent()->index());
}



