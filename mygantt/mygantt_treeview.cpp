#include "mygantt_treeview.h"
#include "mygantt_headerview.h"

#include "mygantt_globalvalues.h"
#include "mygantt_graphicsview.h"


#include <QScrollBar>

#include <QDebug>

GanttTreeView::GanttTreeView(QWidget * parent )
{
    setHeader(new GanttHeaderView(Qt::Horizontal,this));
    setMinimumWidth(GANTTTREEVIEW_MIN_WIDTH);
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

void GanttTreeView::repaintHeader()
{
    qDebug() << "GanttTreeView::repaintHeader()";

    GanttHeaderView *p_header = dynamic_cast <GanttHeaderView*>(header());

    if(p_header)
        p_header->update();

    update();
}


