#ifndef GANTTSCENE_H
#define GANTTSCENE_H


#include "mygantt_header.h"
#include "mygantt_slider.h"

#include <QGraphicsScene>
#include <QMap>

class GanttGraphicsView;
class GanttItem;

class QModelIndex;

class GanttScene : public QGraphicsScene
{


public:
    GanttScene(QObject * parent = 0);


    void updateWidth(int w);
    void updateHeight(int h);

    void drawBackground(QPainter *painter, const QRectF &rect); ///< Задник план содержит сетку



    void drawForeground(QPainter *painter, const QRectF &rect); ///< Перендий план содержит шапку

    void setMode(GanttHeader::GanttPrecisionMode newMode);

    void addItems(GanttInfoItem* items);
    void addItems(const QList<GanttInfoItem*>& items);

    GanttItem* itemByInfo(const GanttInfoLeaf* key) const;

    void updateHeaderPos(int dy);
    void updateSliderRect();

    void onViewResize(const QSize& newSize);
    void onViewAdded();

    void setHeaderMode(GanttHeader::GanttHeaderMode mode);
    GanttHeader::GanttHeaderMode headerMode() const;

    void changeExpanding(const QModelIndex& index);

    UtcDateTime slidersDt() const;

    GanttSlider *slider() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    void addItemsHelper(GanttInfoItem* item);
    void updateItems();


private:

    QList<GanttItem*> m_items;
    QMap<const GanttInfoLeaf*, GanttItem*> m_itemByInfo;
    GanttHeader *m_header;
    GanttSlider *m_slider;
};

#endif // GANTTSCENE_H
