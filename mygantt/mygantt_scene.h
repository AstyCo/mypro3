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
    Q_OBJECT

public:
    GanttScene(QObject * parent = 0);


    void updateWidth(int w);
    void updateHeight(int h);

    void drawBackground(QPainter *painter, const QRectF &rect); ///< Задний план содержит сетку

    void setMode(GanttHeader::GanttPrecisionMode newMode);

    void addItems(GanttInfoItem* items);
    void addItems(const QList<GanttInfoItem*>& items);

    GanttItem* itemByInfo(const GanttInfoLeaf* key) const;

    void updateHeaderPos(int dy);
    void updateSliderRect();

    void onViewResize(const QSize& newSize);
    void onViewAdded(QGraphicsView* view);

    void setHeaderMode(GanttHeader::GanttHeaderMode mode);
    GanttHeader::GanttHeaderMode headerMode() const;

    void changeExpanding(const QModelIndex& index);

    UtcDateTime slidersDt() const;

    GanttSlider *slider() const;

    void setRange(UtcDateTime min, UtcDateTime max);


    UtcDateTime minDt() const;
    UtcDateTime maxDt() const;
    UtcDateTime startDt() const;
    UtcDateTime finishDt() const;

    UtcDateTime xToDt(qreal x) const;
    qreal dtToX(const UtcDateTime& dt) const;

    UtcDateTime startByDt(const UtcDateTime& dt,GanttHeader::GanttPrecisionMode mode) const;
    UtcDateTime startByDt(const UtcDateTime& dt) const;

    UtcDateTime finishByDt(const UtcDateTime& dt,GanttHeader::GanttPrecisionMode mode) const;
    UtcDateTime finishByDt(const UtcDateTime& dt) const;

    UtcDateTime nextStart(const UtcDateTime& dt,GanttHeader::GanttPrecisionMode mode) const;
    UtcDateTime nextStart(const UtcDateTime& dt) const;

    UtcDateTime prevFinish(const UtcDateTime& dt,GanttHeader::GanttPrecisionMode mode) const;
    UtcDateTime prevFinish(const UtcDateTime& dt) const;

    GanttHeader::GanttPrecisionMode headerPrecisionMode() const;
    GanttHeader::GanttPrecisionMode calculateTimeMode(const UtcDateTime &min, const UtcDateTime &max) const;

    void emitLimitsChanged(const UtcDateTime &start, const UtcDateTime &finish);

signals:
    void limitsChanged(const UtcDateTime &start, const UtcDateTime &finish);
    void viewResized();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    void updateSlider();
    void addItemsHelper(GanttInfoItem* item);
    void updateItems();

private slots:
    void onInfoChanged();

private:

    QList<GanttItem*> m_items;
    QMap<const GanttInfoLeaf*, GanttItem*> m_itemByInfo;
    GanttHeader *m_header;
    GanttSlider *m_slider;

    friend class GanttWidget;
};

#endif // GANTTSCENE_H
