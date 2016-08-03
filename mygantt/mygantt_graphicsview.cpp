#include "mygantt_graphicsview.h"
#include "mygantt_widget.h"
#include "mygantt_scene.h"
#include "mygantt_treeview.h"

#include <QResizeEvent>
#include <QScrollBar>

GanttGraphicsView::GanttGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    initialize();
}


GanttGraphicsView::GanttGraphicsView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene,parent)
{
    initialize();
}


void GanttGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
}

void GanttGraphicsView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);

    if(!m_treeView)
        return;

    int vs = verticalScrollBar()->value();
    m_treeView->verticalScrollBar()->setValue(vs);
    QList<QRectF> rects;
    rects.append(m_treeView->viewport()->rect());
    m_treeView->update();


}


void GanttGraphicsView::initialize()
{
//    setRenderHint(QPainter::Antialiasing,true);
    setRenderHint(QPainter::TextAntialiasing,false);

    setFrameStyle(0);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

}
void GanttGraphicsView::setTreeView(GanttTreeView *treeView)
{
    m_treeView = treeView;
}

void GanttGraphicsView::setScene(GanttScene *scene)
{
    m_scene = scene;
    QGraphicsView::setScene(scene);
}




