#ifndef GANTTTREEVIEW_H
#define GANTTTREEVIEW_H

#include <QTreeView>

class GanttGraphicsView;

class GanttTreeView : public QTreeView
{
public:
    GanttTreeView(QWidget * parent = 0);

    void setGraphicsView(GanttGraphicsView *graphicsView);

    void repaintHeader();

protected:
    void scrollContentsBy(int dx, int dy);

    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *);

private:

    GanttGraphicsView* m_graphicsView;

    int m_hSliderHeight;

    friend class GanttGraphicsView;
    friend class GanttHeaderView;

};

#endif // GANTTTREEVIEW_H
