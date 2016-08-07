#ifndef GANTTITEM_H
#define GANTTITEM_H

#include "mygantt_infoleaf.h"

#include <QGraphicsItem>

class GanttScene;

class GanttItem : public QGraphicsItem
{
public:
    GanttItem(GanttInfoLeaf *info,QGraphicsItem *parent = 0);

    //
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setScene(GanttScene *scene);
    void setBoundingRectSize(const QSizeF &boundingRectSize);

    GanttInfoLeaf *info() const;

    QRectF rect() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    GanttInfoLeaf* m_info;
    GanttScene *m_scene;

    QSizeF m_boundingRectSize;


};

#endif // GANTTITEM_H
