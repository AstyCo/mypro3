#include "mygantt_treeview.h"

#include "mygantt_graphicsview.h"

#include <QScrollBar>

GanttTreeView::GanttTreeView(QWidget * parent )
{

}

void GanttTreeView::scrollContentsBy(int dx, int dy)
{
    QTreeView::scrollContentsBy(dx,dy);

    if(!m_graphicsView)
        return;

    int vs = verticalScrollBar()->value();
    m_graphicsView->verticalScrollBar()->setValue(vs);
    QList<QRectF> rects;
    rects.append(m_graphicsView->viewport()->rect());
    m_graphicsView->updateScene(rects);

}
void GanttTreeView::setGraphicsView(GanttGraphicsView *graphicsView)
{
    m_graphicsView = graphicsView;
}


