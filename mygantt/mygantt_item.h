#ifndef GANTTITEM_H
#define GANTTITEM_H

#include "mygantt_infoleaf.h"


#include <QGraphicsObject>

class GanttHeader;
class GanttScene;

class GanttItem : public QGraphicsObject
{
    Q_OBJECT

public:
    GanttItem(GanttInfoLeaf *info,QGraphicsItem *parent = 0);

    //
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



    GanttInfoLeaf *info() const;

    QRectF rect() const;

    void setHeader(GanttHeader *header);

public slots:
    void setScene(GanttScene *scene);
    void setBoundingRectSize(const QSizeF &boundingRectSize);

    void updateGeometry();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:

    GanttHeader* m_header;
    GanttInfoLeaf* m_info;
    GanttScene *m_scene;

    QSizeF m_boundingRectSize;


};

#endif // GANTTITEM_H
