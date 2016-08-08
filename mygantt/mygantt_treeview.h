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

private:

    GanttGraphicsView* m_graphicsView;

    friend class GanttGraphicsView;

};

#endif // GANTTTREEVIEW_H
