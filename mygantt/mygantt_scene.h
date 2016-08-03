#ifndef GANTTSCENE_H
#define GANTTSCENE_H


#include "mygantt_header.h"

#include <QGraphicsScene>
#include <QMap>

class GanttGraphicsView;
class GanttItem;

class GanttScene : public QGraphicsScene
{


public:
    GanttScene(QObject * parent = 0);

    void updateWidth(qreal w);
    void updateHeight(qreal h);

    void drawBackground(QPainter *painter, const QRectF &rect);

    void setMode(GanttHeader::GanttPrecisionMode newMode);

    void addItems(GanttInfoItem* items);
    void addItems(const QList<GanttInfoItem*>& items);

    GanttItem* itemByInfo(const GanttInfoLeaf* key) const;

    void updateHeaderPos(int dy);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    void addItemsHelper(GanttInfoItem* item);

private:

    QList<GanttItem*> m_items;
    QMap<const GanttInfoLeaf*, GanttItem*> m_itemByInfo;
    GanttHeader *m_header;
};

#endif // GANTTSCENE_H
