#include "mygantt_scene.h"

#include "mygantt_graphicsview.h"
#include "mygantt_item.h"

#include "mygantt_infoleaf.h"
#include "mygantt_infonode.h"


#include <QGraphicsView>

#include <QScrollBar>
#include <QSize>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>

GanttScene::GanttScene(QObject * parent) :
    QGraphicsScene(parent)
{
    m_header = new GanttHeader;
    m_header->setScene(this);
    m_slider = new GanttSlider;
    m_slider->setScene(this);

    updateSliderRect();

//    QBrush backgroundBrush(Qt::HorPattern);
//    backgroundBrush.setMatrix(QMatrix().scale(1,DEFAULT_ITEM_HEIGHT*1.0/15));

//    setBackgroundBrush(backgroundBrush);


}

void GanttScene::updateWidth(int w)
{
    setSceneRect(0,0,w,sceneRect().height());

    if(views().isEmpty())
        return;

    if(m_header->headerMode() == GanttHeader::TimelineMode)
        return;

    GanttGraphicsView* p_view = dynamic_cast<GanttGraphicsView*>(views()[0]);

    if(p_view)
    {
        p_view->setMaximumWidth(w);
    }

}

void GanttScene::updateHeight(int h)
{
    setSceneRect(0,0,sceneRect().width(),h);
}

void GanttScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter,rect);


    qreal   /*bgTop = rect.top()
            ,*/bgBottom = rect.bottom()
            ,bgLeft = rect.left()
            ,bgRight = rect.right();

    QPen pen;
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);

//    qreal  startX = (qCeil(rect.x() / m_header->secondHeaderWidth()))*m_header->secondHeaderWidth();
//    while(startX<bgRight)
//    {
//        painter->drawLine(QPointF(startX,bgTop),QPointF(startX,bgBottom));
//        startX+=m_header->secondHeaderWidth();
//    }

    qreal startY = (qCeil(rect.y() / DEFAULT_ITEM_WIDTH))*DEFAULT_ITEM_WIDTH;

    while(startY<bgBottom)
    {
        painter->drawLine(QPointF(bgLeft,startY),QPointF(bgRight,startY));
        startY+=DEFAULT_ITEM_WIDTH;
    }
}

void GanttScene::drawForeground(QPainter *painter, const QRectF &rect)
{

//    qDebug() <<"scene rect: "<<sceneRect();

    QGraphicsScene::drawForeground(painter,rect);

//    if(m_header->headerMode() == GanttHeader::TimelineMode)
//    {
//        QRect headerRect(rect.left(),rect.top(),rect.width(),DEFAULT_HEADER_HEIGHT);
//        qDebug() << "GanttScene::drawForeground " << headerRect;
//        // Header background
//        QLinearGradient linearGradient(QPointF(rect.left(),rect.top()),QPointF(rect.left(),rect.top() + DEFAULT_HEADER_HEIGHT));
//        linearGradient.setColorAt(0, Qt::cyan);
//        linearGradient.setColorAt(0.4, Qt::white);
//        linearGradient.setColorAt(0.6, Qt::white);
//        linearGradient.setColorAt(1, Qt::cyan);
//        painter->fillRect(headerRect, linearGradient);
//        // Center horizontal line
//        painter->drawLine(QPointF(0,DEFAULT_ITEM_HEIGHT),QPointF(rect.right(),DEFAULT_ITEM_HEIGHT));
//        // Little and big hatches(with text)



//    }
}

void GanttScene::setMode(GanttHeader::GanttPrecisionMode newMode)
{
    m_header->setMode(newMode);

    updateItems();
}

void GanttScene::addItems(GanttInfoItem* item)
{
    m_header->onItemsAddition(item);
    addItemsHelper(item);
}

void GanttScene::addItems(const QList<GanttInfoItem *> &items)
{
    m_header->onItemsAddition(items);

    foreach(GanttInfoItem* item, items)
        addItemsHelper(item);
}

GanttItem *GanttScene::itemByInfo(const GanttInfoLeaf * key) const
{
    return m_itemByInfo.value(key);
}

void GanttScene::updateHeaderPos(int dy)
{
    m_header->setPos(m_header->scenePos().x(),/*m_header->scenePos().y()+*/dy);
}

void GanttScene::onViewResize(const QSize&newSize)
{
    if(m_header->headerMode() == GanttHeader::TimelineMode)
    {
        if(!views().isEmpty())
        {
            updateWidth(views()[0]->viewport()->width());
        }
        m_header->setCurrentWidth(newSize.width());
        updateItems();
    }

    updateSliderRect();
}

void GanttScene::onViewAdded()
{
    m_header->init();
}

void GanttScene::setHeaderMode(GanttHeader::GanttHeaderMode mode)
{
    m_header->setHeaderMode(mode);
}

GanttHeader::GanttHeaderMode GanttScene::headerMode() const
{
    return m_header->headerMode();
}

void GanttScene::changeExpanding(const QModelIndex &index)
{
    if(views().isEmpty())
        return;
    GanttGraphicsView* p_view = dynamic_cast<GanttGraphicsView*>(views()[0]);
    if(!p_view)
        return;

    p_view->changeExpanding(index);
}


void GanttScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    m_header->setPos(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}


void GanttScene::addItemsHelper(GanttInfoItem *item)
{
    GanttInfoLeaf *leaf = dynamic_cast<GanttInfoLeaf*>(item);
    if(leaf)
    {
        GanttItem *p_item = new GanttItem(leaf);


        qreal startPos = m_header->dtToX(leaf->start()),
              itemWidth = m_header->dtToX(leaf->finish()) - startPos;

        m_items.append(p_item);
        m_itemByInfo.insert(leaf,p_item);
        p_item->setScene(this);
        p_item->setBoundingRectSize(QSizeF(itemWidth, DEFAULT_ITEM_WIDTH));
        p_item->setPos(startPos, 2*DEFAULT_ITEM_WIDTH + leaf->pos());
    }
    else
    {
        GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(item);
        if(node)
        {
            foreach(GanttInfoItem* item, node->m_items)
                addItemsHelper(item);
        }
    }
}

void GanttScene::updateItems()
{
    for(int i = 0; i < m_items.size(); ++i)
    {
        GanttInfoLeaf* p_info = m_items[i]->info();
        qreal startPos = m_header->dtToX(p_info->start()),
              itemWidth = m_header->dtToX(p_info->finish()) - startPos;
        qreal top = m_items[i]->rect().top(),
                height = m_items[i]->rect().height();


        m_items[i]->setPos(startPos, top);
        m_items[i]->setBoundingRectSize(QSizeF(itemWidth, height));
    }


    update();
}

void GanttScene::updateSliderRect()
{
    if(!m_slider || !m_header)
        return;

    if(m_header->headerMode() == GanttHeader::TimelineMode)
    {
        if(!views().isEmpty())
        {
            QGraphicsView* p_view = views()[0];


            m_slider->setSlidersRect(QRectF( MIN_WIDTH_FOR_TIME_VISUALIZING/2
                                     ,p_view->verticalScrollBar()->value()
                                     ,width()-MIN_WIDTH_FOR_TIME_VISUALIZING
                                     ,p_view->height()
                                     ));
        }
    }

}

